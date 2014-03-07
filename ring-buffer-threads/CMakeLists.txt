PROJECT(RingBuffer)
cmake_minimum_required(VERSION 2.8)

INCLUDE_DIRECTORIES(${RingBuffer_SOURCE_DIR})
LIST( APPEND CMAKE_CXX_FLAGS "-std=c++11 -pthread" )

SET(SRC
        main.cpp
   )

ADD_EXECUTABLE(ring-buffer ${SRC})
TARGET_LINK_LIBRARIES(ring-buffer rt)
