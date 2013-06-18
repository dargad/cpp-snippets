PROJECT(BoostInterprocess)
cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(Boost) # COMPONENTS interprocess

IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} ${BoostInterprocess_SOURCE_DIR}/include)
    ADD_DEFINITIONS( "-DHAS_BOOST" )

    SET(SRC_CLIENT
        client.cpp
    )

    SET(SRC_SERVER
        server.cpp
    )

    SET(EXECS client target)

    FOREACH(exec ${EXECS})

    ADD_EXECUTABLE(${exec} ${SRC_CLIENT})

    IF(UNIX)
        TARGET_LINK_LIBRARIES(${exec} rt)
    ENDIF(UNIX)

    ENDFOREACH(exec)


ELSE(Boost_FOUND)
MESSAGE("Finding boost failed :(")
ENDIF()
