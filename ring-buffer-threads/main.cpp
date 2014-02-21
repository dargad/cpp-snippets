#include <ring_buffer.h>

#include <exception>
#include <future>
#include <iostream>

RingBuffer<int> ringBuffer;

void producerThread()
{
    std::cout << "[producer] begin" << std::endl;
    for (int i=0; i<1099; ++i)
    {
        std::cout << "[producer] " << i << std::endl;
        ringBuffer.write(i);
    }
    std::cout << "[producer] end" << std::endl;
}

void consumerThread()
{
    std::cout << "[consumer] begin" << std::endl;
    for (int i=0; i<1; ++i)
    {
        std::cout << "[consumer] " << ringBuffer.read() << std::endl;
    }
    std::cout << "[consumer] end" << std::endl;
}

int main(int argc, char const *argv[])
{
    auto f_prod1 (std::async(std::launch::async, producerThread));

    std::vector<std::shared_future<void>> consumers;
    for (int i=0; i<10; ++i)
    {
        std::shared_future<void> consumer(std::async(std::launch::async, consumerThread));
        consumers.push_back(consumer);
    }

    f_prod1.wait();

    for (auto consumer : consumers)
    {
        consumer.wait();
    }

    return 0;
}
