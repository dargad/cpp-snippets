#include "shm.h"
#include <iostream>

int main() {
    SharedMemory<int> shm("test.tmp", SharedMemory<int>::Server);
    int read = shm.read();
    std::cout << "Reading " << read << " from shared memory" << std::endl;
}
