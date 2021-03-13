#include <iostream>
#include "./src/Info/InfoManager.hpp"
#include "./src/Info/MemorySpace.hpp"
#include "./src/Info/DiskSpace.hpp"
#include "./src/Info/CoreTemp.hpp"
#include "./src/Info/LogInfo.hpp"
#include "./src/SSL/ClientSSL.hpp"

#define DEVICE_QUID     "4cbb4ef1-f36c-476d-bf50-ae04f65f8537"
#define CLIENT_TIME     1000000//1sec
#define SERVER_IP       "192.168.1.33"
#define SERVER_PORT     8000

int main(void)
{
    LogInfo m_log(DEVICE_QUID);
    MemorySpace m_memory;
    DiskSpace m_disk;
    CoreTemp m_core;
   
    InfoManager m_manager(DEVICE_QUID);
    m_manager.AddInfo(dynamic_cast<Info*>(&m_memory));
    m_manager.AddInfo(dynamic_cast<Info*>(&m_disk));
    m_manager.AddInfo(dynamic_cast<Info*>(&m_core));

    ClientSSL m_client(SERVER_IP, SERVER_PORT);
    m_client.Init();
    
    while(true)
    {
        m_manager.GetInfo();
        m_client.SendInfo(m_manager);
        m_client.SendLog(m_log)
        usleep(CLIENT_TIME);
    }
    
    m_client.Close();
     
    return EXIT_SUCCESS;
}