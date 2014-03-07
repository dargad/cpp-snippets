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

#include <ring_buffer.h>

#include <exception>
#include <future>
#include <iostream>
#include <thread>
#include <mutex>

RingBuffer<int> ringBuffer;

/**
 * Producer thread - inserts items to the ring buffer.
 */
void producerThread()
{
    std::cout << "[producer] begin" << std::endl;
    int counter(0);
    while (true)
    {
        std::cout << "[producer] " << counter << std::endl;
        ringBuffer.write(++counter);
    }
    std::cout << "[producer] end" << std::endl;
}

/**
 * Consumer thread - removes items from the ring buffer.
 */
void consumerThread()
{
    std::cout << "[consumer] begin" << std::endl;
    while (true)
    {
        std::cout << "[consumer] " << ringBuffer.read() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "[consumer] end" << std::endl;
}

int main(int argc, char const *argv[])
{
    auto f_prod1 (std::async(std::launch::async, producerThread));

    const int numberOfConsumers(10);
    // Create a number of consumers
    std::vector<std::shared_future<void>> consumers;
    for (int i=0; i<numberOfConsumers; ++i)
    {
        std::shared_future<void> consumer(std::async(std::launch::async, consumerThread));
        consumers.push_back(consumer);
    }

    // The threads will never exit so there is no point in waiting for them

    return 0;
}
