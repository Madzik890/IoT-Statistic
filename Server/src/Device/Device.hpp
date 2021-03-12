#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "../Definitions.hpp"
#include <string>

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
    Device(const std::string guid, const Core core, const Memory memory, const Disk disk);
    ~Device();
    
    const Core GetCore();
    const Memory GetMemory();
    const Disk GetDisk();
    const std::string GetGuid();
private:
    Core m_core;
    Memory m_memory;
    Disk m_disk;
    std::string s_guid;
    bool b_logSent;
};

#endif /* DEVICE_HPP */

