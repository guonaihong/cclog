#ifndef __CCQUEUEU_H
#define __CCQUEUEU_H

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class Queue {
    public:
        Queue(){}

        //Queue(const Queue&);

        Queue& operator=(const Queue&) = delete;

        void push(T new_value) {
            std::lock_guard<std::mutex> L(this->mut);
            this->data_queue.push(new_value);
            this->data_cond.notify_one();
        }

        void wait_and_pop(T& value) {
            std::unique_lock<std::mutex> L(mut);
            this->data_cond.wait(L, [this]{ return !this->data_queue.empty(); });
            value = this->data_queue.front();
            this->data_queue.pop();
        }

        bool empty() const {
            return this->data_queue.empty();
        }

    private:
        std::mutex mut;
        std::queue<T> data_queue;
        std::condition_variable data_cond;
};

#endif
