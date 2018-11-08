#ifndef __CCFILE_WRITE_H
#define __CCFILE_WRITE_H

#include "ccqueue.h"
#include <thread>

using string = std::string;

const string CCFILE_DEFAULT = "default";

const int CCFILE_KB = 1024;
const int CCFILE_MB = CCFILE_KB * 1024;
const int CCFILE_GB = CCFILE_KB * 1024;

const int CCFILE_DEFAULT_MAX_SIZE = 100 * CCFILE_MB;

class CCFile {

    public:
        CCFile(string prefix, string dir, int max_size, int max_archive);

        int write(const char *buf, size_t count);

    private:
        void   compression_main();
        void   sort_and_del_main();
        int    max_size;
        int    max_archive;

        string dir;
        string prefix;
        
        int    fd;

        Queue<std::string>  qcompress;
        Queue<bool>         qsort_and_del;

        std::thread sort_and_del;
        std::thread compression;
};

string base(string path) {
    if (path == "") {
        return ".";
    }

    auto is_path_separator = [](char c)-> bool{
        return c == '/';
    };

    while(path.size() > 0 && is_path_separator(path.c_str()[path.size()-1])) {
        path.resize(path.size() - 1);
    }

    auto i = long(path.size() -1);

    while(i >= 0 && !is_path_separator(path.c_str()[i])) {
        i--;
    }

    if (i >= 0) {
        return path.c_str() + i + 1;
    }

    if (path == "") {
        return "/";
    }

    return "";
}

CCFile::CCFile(string prefix, string dir, int max_size, int max_archive) {
    if (prefix == "") {
        prefix = CCFILE_DEFAULT;
    }

    if (max_size == 0) {
        max_size = CCFILE_DEFAULT_MAX_SIZE;
    }

    this->compression = std::thread([this]{
        this->compression_main();
    });

    this->sort_and_del = std::thread([this] {
        this->sort_and_del_main();
    });

    if (dir.size() > 0) {

        char last = dir.c_str()[dir.size()-1]; 
        if(last != '/' && last != '\\') {
        }
    }
}

void CCFile::compression_main() {

    for (;;) {
    }
}

void CCFile::sort_and_del_main() {

    for (;;) {

        bool ok;
        this->qsort_and_del.wait_and_pop(ok);

    }
}

int CCFile::write(const char * buf, size_t count) {
    return 0;
}

#endif
