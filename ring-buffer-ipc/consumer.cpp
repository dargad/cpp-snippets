#include <ring_buffer.h>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    using namespace boost::interprocess;
    shared_memory_object shm(open_only, "ringbuffer", read_write);
    mapped_region region(shm, read_write);
    RingBuffer<int> * buff = static_cast<RingBuffer<int>*>(region.get_address());
    std::cout << "buff->read(): " << buff->read() << std::endl;
    return 0;
}
