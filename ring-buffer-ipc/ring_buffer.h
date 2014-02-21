#ifndef RINGBUFF_H
#define RINGBUFF_H value

#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include <atomic>
#include <iostream>
#include <vector>

template <typename DataType, int size=10>
class RingBuffer
{
public:
    RingBuffer(): m_readPointer{0}, m_writePointer{0} {};

    DataType read();
    void write(const DataType& data);
private:
    int nextPointer(int currentPointer) { return (currentPointer + 1) % size; }

    boost::interprocess::interprocess_mutex     m_writeLock;
    boost::interprocess::interprocess_condition m_canWrite;

    boost::interprocess::interprocess_mutex     m_readLock;
    boost::interprocess::interprocess_condition m_canRead;

    std::atomic<int>                            m_readPointer;
    std::atomic<int>                            m_writePointer;
    DataType                                    m_data[size];
};




template <typename DataType, int size>
inline
DataType RingBuffer<DataType, size>::read()
{
    boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(m_readLock);
    if (m_readPointer == m_writePointer)
    {
        std::cout << "[waiting] m_writePointer: " << m_writePointer << "\tm_readPointer: " << m_readPointer << std::endl;
        m_canRead.wait(lock);
    }
    DataType result {m_data[int(m_readPointer)]};
    m_readPointer = nextPointer(m_readPointer);
    m_canWrite.notify_one();
    return result;
}

template <typename DataType, int size>
inline
void RingBuffer<DataType, size>::write(const DataType& data)
{
    boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(m_writeLock);
    auto nextWritePtr(nextPointer(m_writePointer));
    if (m_readPointer == nextWritePtr)
    {
        std::cout << "[waiting] m_writePointer: " << m_writePointer << "\tm_readPointer: " << m_readPointer << std::endl;
        m_canWrite.wait(lock);
    }
    m_data[m_writePointer] = data;
    m_writePointer = nextWritePtr;
    m_canRead.notify_one();
}

#endif // RINGBUFF_H
