#include "ccfile_write.h"

int main() {
    CCFile w = {"default", "./my-test/", 10, 1024};
    w.write("hello world", 8);
    return 0;
}
