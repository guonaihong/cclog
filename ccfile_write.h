#ifndef __CCFILE_WRITE_H
#define __CCFILE_WRITE_H

#include "ccqueue.h"
#include <thread>

using string = std::string;

class CCFile {

    public:
        CCFile(string prefix, string dir, int max_size, int max_archive);

        int write(const char *buf, size_t count);

    private:
        void   write_file_main();
        void   compression_main();
        int    max_size;
        int    max_archive;
        string dir;
        string prefix;
        
        int    fd;

        std::thread write_file;
        std::thread compression;
};

CCFile::CCFile(string prefix, string dir, int max_size, int max_archive) {
    this->write_file = std::thread([this] {
        this->write_file_main();
    });

    this->compression = std::thread([this]{
        this->compression_main();
    });
}

void CCFile::write_file_main() {
}

void CCFile::compression_main() {
}

int CCFile::write(const char * buf, size_t count) {
    return 0;
}

#endif
