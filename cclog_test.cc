#include <iostream>
#include "cclog.h"

int main() {
    CCLog log = {"debug", "cclog"};
    std::cout << log.print_buf() << std::endl;
    return 0;
}
