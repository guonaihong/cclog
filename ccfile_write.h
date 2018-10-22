#ifndef __CCFILE_WRITE_H
#define __CCFILE_WRITE_H

class CCFile {

    public:
        CCFile(string prefix, string dir, int max_size, int max_archive);

        int write(const char *buf, size_t count);

    private:
        int    max_size;
        int    max_archive;
        string dir;
        string prefix;
        
        int    fd;
};

#endif
