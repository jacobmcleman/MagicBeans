#include "MT_Print.h"

#include <iostream>
#include <mutex>

namespace
{
    std::mutex printMutex;
}

void PrintMessage(std::string message)
{
    printMutex.lock();

    std::cout << message << std::endl;

    printMutex.unlock();
}
