#include <iostream>
#include "cclog.h"

int main() {
    CCLog log = {Level::Debug, "cclog"};

    log.errorf("hello world hello worlhello worlhello worlhello worlhello #\n");
    log.infof("hello world hello worlhello worlhello worlhello worlhello #\n");
    log.warnf("hello world hello worlhello worlhello worlhello worlhello #\n");
    log.debugf("hello world hello worlhello worlhello worlhello worlhello #\n");
    return 0;
}
