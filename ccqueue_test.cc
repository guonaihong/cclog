#include "ccqueue.h"
#include <string>
#include <thread>
#include <iostream>

int main() {
    //Queue<std::string> q;
    Queue<std::string> q();
    std::thread p([&]{
            for (int i = 0; i < 1000; i++) {
                std::string s = std::to_string(i);
                q.push(s);
            }
    });

    p.join();

    std::thread c([&]{
            for (;;) {
                std::string s;
                q.wait_and_pop(s);
                std::cout << s << std::endl;
            }
    });

    c.join();
    return 0;
}
