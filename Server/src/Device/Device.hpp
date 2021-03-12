#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "../Definitions.hpp"

struct Core
{
    int i_coreTemp;
};

struct Memory
{
    ull u_memoryTotal;
    ull u_memoryAvaible;
    ull u_memoryFree;
};

struct Disk
{
    ull u_diskTotal;
    ull u_diskFree;
};

class Device
{
public: 
    Device(std::string buffer);
    ~Device();
    
private:
    Core m_core;
    Memory m_memory;
    Disk m_disk;
}

#endif /* DEVICE_HPP */

