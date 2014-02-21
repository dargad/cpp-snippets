#include <ring_buffer.h>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <iostream>
#include <random>

int main(int argc, char const *argv[])
{
    using namespace boost::interprocess;

    struct shm_remove
    {
        shm_remove() { shared_memory_object::remove("ringbuffer"); }
        ~shm_remove(){ shared_memory_object::remove("ringbuffer"); }
    } remover;

    RingBuffer<int> * buff = nullptr;

    //try {
        //std::cout << "trying to open..." << std::endl;
        //shared_memory_object shm(open_only, "ringbuffer", read_write);
        //mapped_region region(shm, read_write);
        //buff = static_cast<RingBuffer<int>*>(region.get_address());
    //} catch (boost::interprocess::interprocess_exception& e) {
        std::cout << "creating..." << std::endl;
        shared_memory_object shm(open_or_create, "ringbuffer", read_write);
        shm.truncate(sizeof(RingBuffer<int>));
        mapped_region region(shm, read_write);
        buff = new (region.get_address()) RingBuffer<int>;
    //}

    std::mt19937 randGen;
    std::uniform_int_distribution<int> int_dist1000(0, 1000);

    while (true)
    {
        auto num(int_dist1000(randGen));
        buff->write(num);
        std::cout << "buff->write(" << num << ") " << std::endl;
    }
    return 0;
}
