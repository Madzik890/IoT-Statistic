#include "DataBase.hpp"
#include <fstream>

int saveXML(Device device)
{
    std::string s_path = "./dataBase/info/" + device.GetGuid();
    std::ofstream m_deviceFile(s_path.c_str());
    if(m_deviceFile.is_open())
    {
        m_deviceFile << "GUID: " << device.GetGuid() << std::endl;
    
        m_deviceFile << "MemorySpace Avaible: " << device.GetMemory().u_memoryAvaible << std::endl;
        m_deviceFile << "MemorySpace Free: " << device.GetMemory().u_memoryFree << std::endl;
    
        m_deviceFile << "DiskSpace Total: " << device.GetDisk().u_diskTotal << std::endl;
        m_deviceFile << "DiskSpace Free: " << device.GetDisk().u_diskFree << std::endl;
    
        m_deviceFile << "Core temp: " << device.GetCore().i_coreTemp << std::endl;
    
        m_deviceFile.close();
        
        return SERVER_SUCCESS;
    }
    return SERVER_ERROR;
}

int saveXMLLog(std::string guid, std::string log)
{
    std::string s_path = "./dataBase/log/" + guid;
    std::ofstream m_deviceFile(s_path.c_str());
    if(m_deviceFile.is_open())
    {
        m_deviceFile << log;
        m_deviceFile.close();
        
        return SERVER_SUCCESS;
     }
    return SERVER_ERROR;
}

