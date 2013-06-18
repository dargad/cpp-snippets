DESCRIPTION
shm.h - template-based implementation of a shared memory wrapper.
    Making it a template allows to support all data types that
	have a copy constructor and assignment operator.

server.cpp - implementation of the server process:
  - creates file to be memory-mapped
  - initializes shared memory
  - waits to read from client

client.cpp - implementation of the client process:
  - opens shared memory
  - writes a number provided as command line argument (or 123 if nothing was provided) to memory

CONFIGURE AND BUILD
There is a already prepared Visual Studio solution (BoostInterprocess.sln) in the 'build' subdirectory.
However, it may require readjusting boost path in the project settings (for both: client and server).

Alternatively there is also a possiblity to prepare a Visual Studio sln from scratch:
1. Install cmake.
2. Run cmd.exe.
3. Go to the project directory (where the CMakeLists.txt file is).
4. Run cmake -G "Visual Studio 11 Win64" -DCMAKE_INCLUDE_PATH="/path/to/your/boost/installation"
5. There will be a BoostInterprocess.sln in current working dir
	which can be simply opened with Visual Studio

USAGE
1. Start server (server process will hang waiting for input).
2. Start client providing an integer argument:
	client.exe 123
3. Client will produce output:
	Writing 123
4. Server process will unblock and write:
    Reading 123
5. Both processes will exit.