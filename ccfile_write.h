#ifndef __CCFILE_WRITE_H
#define __CCFILE_WRITE_H

#include "ccqueue.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <thread>
#include <functional>
#include <libgen.h>

using string = std::string;

const string CCFILE_DEFAULT = "default";

const int CCFILE_KB = 1024;
const int CCFILE_MB = CCFILE_KB * 1024;
const int CCFILE_GB = CCFILE_KB * 1024;

const int CCFILE_DEFAULT_MAX_SIZE = 100 * CCFILE_MB;

enum class CompressType {Gzip, NotCompress};

class CCFile {

    public:
        CCFile(string prefix, string dir, enum CompressType compress, int max_size, int max_archive);

        int write(const char *buf, size_t count);

        void wait();

    private:
        int    check_size(int size);
        int    open_new();
        void   compression_main();
        void   sort_and_del_main();
        string default_file_name();
        int    max_size;
        int    max_archive;

        enum CompressType compress;

        string dir;
        string prefix;
        string curr_name;
        string default_name;
        
        int    fd;

        Queue<std::string>  qcompress;
        Queue<bool>         qsort_and_del;

        std::thread sort_and_del;
        std::thread compression;
};

static int mkdir_all(const char *path, mode_t mode) {
    std::function<int (const char *, const char *)> mkdir_core = [=](const char *path, const char *start_dir) -> int {
        char *p = strchr((char *)start_dir, '/');
        if (p != NULL) {
            *p = '\0';
            if (strcmp(path, ".") && mkdir(path, mode) == -1) {
                printf("%s:%s\n", strerror(errno), path);
                return -1;
            }

            *p = '/';

            return mkdir_core(path, p + 1);
        }

        return 0;
    };

    if (strchr(path, '/') != nullptr) {
        char *p2 = strdup(path);
        mkdir_core(p2, p2);
        free(p2);
        return 0;
    }

    return mkdir((char *)path, mode);
}

CCFile::CCFile(string prefix, string dir, enum CompressType compress, int max_size, int max_archive) {
    if (prefix == "") {
        prefix = CCFILE_DEFAULT;
    }

    if (max_size == 0) {
        max_size = CCFILE_DEFAULT_MAX_SIZE;
    }

    string name = "";
    if (dir.size() > 0) {

        char last = dir.c_str()[dir.size()-1]; 
        if(last != '/' && last != '\\') {
            name = basename((char *)dir.c_str());
            if (name == ".") {
                name = "";
            } else {
                name += ".log";
            }
        }
    }

    dir = dirname((char *)dir.c_str());

    struct stat sb;
    int rv = stat(dir.c_str(), &sb);
    if (rv == -1) {
        if(ENOENT == errno) {
            mkdir_all(dir.c_str(), 0755);
        }
    }

    this->dir          = dir;
    this->default_name = name;
    this->compress     = compress;
    this->prefix       = prefix;
    this->max_size     = max_size;
    this->max_archive  = max_archive;
    this->fd           = -1;

    this->compression = std::thread([this]{
        this->compression_main();
    });

    this->sort_and_del = std::thread([this] {
        this->sort_and_del_main();
    });

}

void CCFile::compression_main() {

    for (;;) {
        string filename;
        this->qcompress.wait_and_pop(filename);
    }
}

void CCFile::sort_and_del_main() {

    for (;;) {

        bool ok;
        this->qsort_and_del.wait_and_pop(ok);

    }
}

string CCFile::default_file_name() {
    char out[PATH_MAX] = "";
    string name = this->dir + "/" + this->prefix + ".log";

    if (this->default_name != "") {
        name = this->dir + "/" + this->prefix + this->default_name;
    }

    realpath(name.c_str(), out);
    this->curr_name = out;
    return string(out);
}

int CCFile::open_new() {
    if (this->fd != -1) {
        close(this->fd);
        this->fd = -1;
    }

    this->fd = open(this->default_file_name().c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (this->fd == -1) {
        return -1;
    }

    return 0;
}

// return 0 success
// return != 0 error
int CCFile::check_size(int size) {
    if (size > this->max_size) {
        return -1;
    }

    if (this->fd == -1) {
        if (this->open_new() != 0) {
            return -1;
        }
        return 0;
    }

    struct stat sb;

    if (stat(this->curr_name.c_str(), &sb) != 0) {
        return -1;
    }

    return 0;
}

int CCFile::write(const char * buf, size_t count) {
    return 0;
}

void CCFile::wait() {
    this->sort_and_del.join();
    this->compression.join();
}

#endif
