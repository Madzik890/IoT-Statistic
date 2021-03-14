#include "Device.hpp"

Device::Device(const std::string guid, const Core core, const Memory memory, const Disk disk)
:s_guid(guid), m_core(core), m_memory(memory), m_disk(disk)
{
}

Device::~Device()
{
}

const Core Device::GetCore()
{
    return m_core;
}

const Memory Device::GetMemory()
{
    return m_memory;
}

const Disk Device::GetDisk()
{
    return m_disk;
}

const std::string Device::GetGuid()
{
    return s_guid;
}
