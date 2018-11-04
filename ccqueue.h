#ifndef __CCQUEUEU_H
#define __CCQUEUEU_H

#include <queue>

template<typename T>
class Queue {
    public:
        Queue();

        Queue& operator=( const Queue&) = delete;

        void push(T new_value) {
        }

        void wait_and_pop(T& value) {
        }

        bool empty() const {
        }

    private:
        std::mutex mut;
        std::queue<T> data_queue;
        std::condition_variable data_cond;

};

#endif
