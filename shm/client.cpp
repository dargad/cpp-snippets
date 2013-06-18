#include "shm.h"
#include <iostream>
#include <sstream>

int main(int argc, char *argv[]) {
	int toWrite(123);
    if (argc > 1) {
        std::istringstream iss(argv[1]);
        iss >> toWrite;
    }
	SharedMemory<int> shm("test.tmp", SharedMemory<int>::Client);
    shm.write(toWrite);
    std::cout << "Writing " << toWrite << " to shared memory" << std::endl;
}
