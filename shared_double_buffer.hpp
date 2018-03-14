#ifndef SHARED_DOUBLE_BUFFER_HPP
#define SHARED_DOUBLE_BUFFER_HPP

#include <queue>
#include <thread>
#include <condition_variable>
#include "message.hpp"

class SharedDoubleBuffer {
public:
    SharedDoubleBuffer(size_t capacity);
    void push(double sample);
    bool pop(double &popped_sample);
    bool empty();
    std::size_t size();

private:
    std::queue<double> m_write_queue;
    std::queue<double> m_read_queue;
    size_t m_capacity;

    mutable std::mutex m_mutex;
    std::condition_variable m_cv_swapped;

    bool swap();
};
#endif /* SHARED_DOUBLE_BUFFER_HPP */
