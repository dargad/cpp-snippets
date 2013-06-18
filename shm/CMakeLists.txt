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

	ADD_EXECUTABLE(client ${SRC_CLIENT})
	ADD_EXECUTABLE(server ${SRC_SERVER})
	
ELSE(Boost_FOUND)
MESSAGE("Finding boost failed :(")
ENDIF()