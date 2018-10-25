#include <iostream>
#include "cclog.h"

int main() {
    CCLog log = {Level::Debug, "cclog"};
    std::cout << log.print_buf() << std::endl;
    return 0;
}
