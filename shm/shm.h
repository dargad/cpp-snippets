/*
 * Copyright (c) 2013, Dariusz Gadomski
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

#ifndef SHM_H
#define SHM_H

#include <fstream>
#include <iostream>
#include <string>

#define BOOST_DATE_TIME_NO_LIB
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

template<typename DataType>
class SharedMemory {
    struct Guard {
        Guard()
            :mutex(1), bempty(1), bstored(0)
        {
        }

        // guard access to data
        boost::interprocess::interprocess_semaphore mutex;
        // wait for buffer to be empty
        boost::interprocess::interprocess_semaphore bempty;
        // wait for buffer to be filled with data
        boost::interprocess::interprocess_semaphore bstored;

        // actual buffer for the shared data
        DataType buffer;
    };

    Guard *m_guard;
public:
    enum InstanceId {
        Server,
        Client
    };

    SharedMemory(const std::string& filename, InstanceId instance)
        :m_instance(instance)
        ,m_file(0)
        ,m_filename(filename)
    {
        using namespace boost::interprocess;
        if (m_instance == Server) {
            createFile();
        }
        m_file = new file_mapping(m_filename.c_str(), read_write);

        m_reg = new mapped_region(*m_file, read_write);

        if (m_instance == Server) {
            m_guard = new (m_reg->get_address()) Guard;
        } else {
            m_guard = static_cast<Guard*>(m_reg->get_address());
        }
    }

    virtual ~SharedMemory() {
        if (m_file) {
            if (m_instance == Server) {
                removeFile();
            }
            delete m_reg;
            delete m_file;
        }
    }

    void write(const DataType& data);
    DataType read();

private:
    void createFile();
    void removeFile();

    InstanceId m_instance;
    boost::interprocess::file_mapping *m_file;
    boost::interprocess::mapped_region *m_reg;
    std::string m_filename;
};

template<typename DataType>
void SharedMemory<DataType>::write(const DataType& data)
{
    m_guard->bempty.wait();
    m_guard->mutex.wait();
    m_guard->buffer = data;
    m_guard->mutex.post();
    m_guard->bstored.post();
}

template<typename DataType>
DataType SharedMemory<DataType>::read()
{
    DataType tmp;
    m_guard->bstored.wait();
    m_guard->mutex.wait();
    tmp = m_guard->buffer;
    m_guard->mutex.post();
    m_guard->bempty.post();
    return tmp;
}

template<typename DataType>
void SharedMemory<DataType>::createFile()
{
    using namespace boost::interprocess;
    file_mapping::remove(m_filename.c_str());
    std::filebuf fbuf;
    fbuf.open(m_filename.c_str(), std::ios_base::in | std::ios_base::out
                         | std::ios_base::trunc | std::ios_base::binary);
    //Set the size
    fbuf.pubseekoff(sizeof(DataType), std::ios_base::beg);
    fbuf.sputc(0);
}

template<typename DataType>
void SharedMemory<DataType>::removeFile()
{
    boost::interprocess::file_mapping::remove(m_filename.c_str());
}

#endif /* SHM_H */
