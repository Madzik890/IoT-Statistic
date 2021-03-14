#include <unistd.h>
#include "./src/Info/InfoManager.hpp"
#include "./src/Info/MemorySpace.hpp"
#include "./src/Info/DiskSpace.hpp"
#include "./src/Info/CoreTemp.hpp"
#include "./src/Info/LogInfo.hpp"
#include "./src/SSL/ClientSSL.hpp"

#define CLIENT_TIME     1000000//1sec
#define SERVER_IP       "192.168.1.33"
#define SERVER_PORT     8000

std::string generateUUID();

int main(int argc, char** argv)
{
    std::string s_guid = generateUUID();
    std::string s_serverIP = SERVER_IP;
    int i_serverPort = SERVER_PORT;
    
    if(argc > 1)
    {       
        for(int i = 1; i <= argc - 1;i++)
        {
            if(!strcmp( argv[ i ], "-ip" ) && argc >= (i+1))
                s_serverIP = argv[i+1];
            else
            if(!strcmp( argv[ i ], "-p" ) && argc >= (i+1))
                i_serverPort = std::atoi(argv[i+1]);  
        }
    }
    
    LogInfo m_log(s_guid);
    MemorySpace m_memory;
    DiskSpace m_disk;
    CoreTemp m_core;
   
    InfoManager m_manager(s_guid.c_str());
    m_manager.AddInfo(dynamic_cast<Info*>(&m_memory));
    m_manager.AddInfo(dynamic_cast<Info*>(&m_disk));
    m_manager.AddInfo(dynamic_cast<Info*>(&m_core));

    ClientSSL m_client(s_serverIP.c_str(), i_serverPort);
    m_client.Init();
    
    while(true)
    {
        m_manager.GetInfo();
        m_client.SendInfo(m_manager);
        m_client.SendLog(m_log);
        usleep(CLIENT_TIME);
    }
    
    m_client.Close();
     
    return EXIT_SUCCESS;
}

const std::string CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
std::string generateUUID()
{
  std::string uuid = std::string(36,' ');
  int rnd = 0;
  int r = 0;
  
  uuid[8] = '-';
  uuid[13] = '-';
  uuid[18] = '-';
  uuid[23] = '-';

  uuid[14] = '4';

  for(int i=0;i<36;i++){
    if (i != 8 && i != 13 && i != 18 && i != 14 && i != 23) {
      if (rnd <= 0x02) {
          rnd = 0x2000000 + (std::rand() * 0x1000000) | 0;
      }
      rnd >>= 4;
      uuid[i] = CHARS[(i == 19) ? ((rnd & 0xf) & 0x3) | 0x8 : rnd & 0xf];
    }
  }
  return uuid;
}