/*
 * Copyright (c) 2014, Dariusz Gadomski
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RINGBUFF_H
#define RINGBUFF_H value

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <vector>

template <typename DataType, int size=10>
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
