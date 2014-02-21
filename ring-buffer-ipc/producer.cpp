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
