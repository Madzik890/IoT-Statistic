#include <iostream>
#include "MemorySpace.hpp"
#include "DiskSpace.hpp"
#include "CoreTemp.hpp"
#include "ClientSSL.hpp"

using namespace std;
int main(void)
{
    MemorySpace m_memory;
    if(m_memory.CheckInfo())
    {
        std::cout << "Memory Total: " << m_memory.GetMemoryTotal() << " bytes" << std::endl;
        std::cout << "Memory Free: " << m_memory.GetMemoryFree() << " bytes" << std::endl;
        std::cout << "Memory Avaible: " << m_memory.GetMemoryAvaible() << " bytes" << std::endl;
        std::cout << "Memory XML:\n" << m_memory.ToString() << std::endl;
    }
    else
        std::cout << "Cannot get memory info \n" << std::endl;
    
    DiskSpace m_disk;
    if(m_disk.CheckInfo())
    {
        std::cout << "Disk Total: " << m_disk.GetDiskTotal() << " bytes" << std::endl;
        std::cout << "Disk Free: " << m_disk.GetDiskFree() << " bytes" << std::endl;
        std::cout << "Disk XML:\n" << m_disk.ToString() << std::endl;
    }
    else
        std::cout << "Cannot get disk info \n" << std::endl;
    
    CoreTemp m_core;
    if(m_core.CheckInfo())
    {
        std::cout << "Core temp: " << m_core.GetCoreTemp() << "*C" << std::endl;
        std::cout << "Core XML:\n" << m_core.ToString() << std::endl;
    }
    else
        std::cout << "Cannot get core temp \n" << std::endl;
    
    ClientSSL m_client("192.168.1.33", 8000);
    m_client.Init();
    m_client.SendMessage("Test - SSL");
    m_client.Close();
     
    return 0;
}