#include "shared_double_buffer.hpp"
#include <iostream>

/**
 * Construct our buffer with an initial capacity.
 */
SharedDoubleBuffer::SharedDoubleBuffer(size_t capacity)
{
    m_capacity = capacity;
}

/**
 * A threadsafe push of elements onto the write queue. Blocks
 * if we are full and waits for the read queue to be consumed
 * and swapped.
 */
void SharedDoubleBuffer::push(double sample)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    // We've reached our limit. Block until a swap.
    if(m_write_queue.size() >= m_capacity) {
        m_cv_swapped.wait(lock);
    }

    m_write_queue.push(sample);
}

/**
 * Swap the read and write queue returning true on success.
 * Notify anyone pushing to a maxed out write queue.
 */
bool SharedDoubleBuffer::swap()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    if(m_write_queue.size() == 0) return false;
    std::swap(m_write_queue, m_read_queue);
    m_cv_swapped.notify_one();
    return true;
}

/**
 * Pop the front of the queue and store in popped_sample.
 * Returns true on success. If our read queue is empty
 * attempt to swap with the write. If it too is empty
 * return false.
 */
bool SharedDoubleBuffer::pop(double &popped_sample)
{
    if(m_read_queue.empty() && !swap()) return false;

    popped_sample = m_read_queue.front();
    m_read_queue.pop();
    return true;
}

/**
 * Return true if our queue is empty. Requires checking both.
 */
bool SharedDoubleBuffer::empty()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return (m_write_queue.empty() && m_read_queue.empty());
}

/**
 * The size of a shared buffer is the sum of sizes of its two internal buffers.
 */
std::size_t SharedDoubleBuffer::size()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return (m_write_queue.size() + m_read_queue.size());
}

