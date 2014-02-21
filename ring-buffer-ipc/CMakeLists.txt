PROJECT(RingBuffer)
cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(Boost) # COMPONENTS interprocess
FIND_PACKAGE(Threads)

IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} ${BoostInterprocess_SOURCE_DIR}/include)
    INCLUDE_DIRECTORIES(${RingBuffer_SOURCE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
    LIST( APPEND CMAKE_CXX_FLAGS "-std=c++11" )

    SET(SRC_PRODUCER
        producer.cpp
    )

    SET(SRC_CONSUMER
        consumer.cpp
    )

    ADD_EXECUTABLE(producer ${SRC_PRODUCER})
    TARGET_LINK_LIBRARIES(producer rt ${CMAKE_THREAD_LIBS_INIT})
    ADD_EXECUTABLE(consumer ${SRC_CONSUMER})
    TARGET_LINK_LIBRARIES(consumer rt ${CMAKE_THREAD_LIBS_INIT})

    SET(EXECS producer consumer)
ELSE(Boost_FOUND)
MESSAGE("Finding boost failed :(")
ENDIF()
