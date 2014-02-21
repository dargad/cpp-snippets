#ifndef RINGBUFF_H
#define RINGBUFF_H value

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <vector>

template <typename DataType, int size=100>
class RingBuffer
{
public:
    RingBuffer(): m_readPointer{0}, m_writePointer{0} {};

    DataType read();
    void write(const DataType& data);
private:
    int nextReadPointer() { return (m_readPointer + 1) % size; }
    int nextWritePointer() { return (m_writePointer + 1) % size; }

    std::mutex              m_writeLock;
    std::condition_variable m_canWrite;

    std::mutex              m_readLock;
    std::condition_variable m_canRead;

    std::atomic<int>        m_readPointer;
    std::atomic<int>        m_writePointer;
    DataType                m_data[size];
};




template <typename DataType, int size>
inline
DataType RingBuffer<DataType, size>::read()
{
    std::unique_lock<std::mutex> lock(m_readLock);
    if (m_readPointer == m_writePointer)
    {
        m_canRead.wait(lock);
    }
    DataType result {m_data[int(m_readPointer)]};
    m_readPointer = nextReadPointer();
    m_canWrite.notify_one();
    return result;
}

template <typename DataType, int size>
inline
void RingBuffer<DataType, size>::write(const DataType& data)
{
    std::unique_lock<std::mutex> lock(m_writeLock);
    if (m_readPointer == nextWritePointer())
    {
        m_canWrite.wait(lock);
    }
    m_data[m_writePointer] = data;
    m_writePointer = nextWritePointer();
    m_canRead.notify_one();
}

#endif // RINGBUFF_H
