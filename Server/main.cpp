#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include "src/ServerSSL.hpp"
#include "src/ServerCallback.hpp"
#include "src/Device/Device.hpp"
#include "src/DataBase/DataBase.hpp"

#define SSL_CERT              "./example.crt"          // location of SSL cert
#define KEY                   "./key.key"            // location of SSL key
#define DEFAULT_PORT          8000

int main(int argc, char** argv)
{   
    ServerSSL m_server;
    int i_port = DEFAULT_PORT;
    
    if(argc > 1)
    {       
        for(int i = 1; i <= argc - 1;i++)
        {
            if(!strcmp( argv[ i ], "-p" ) && argc >= (i+1))
                i_port = std::atoi(argv[i+1]);  
        }
    }
    
    if (m_server.Init(i_port, SSL_CERT, KEY) == EXIT_SUCCESS)
    {
        m_server.AttachCallback(&DeviceInfoReceived);//ServerCallback.hpp
        dataBaseSave = &saveXML;
        dataBaseLogSave = &saveXMLLog;
        
        while(1)    
        {
            int i_status = m_server.WaitForRequestAndProcess();
            if (i_status != SERVER_SUCCESS && i_status != SERVER_ERROR_WOULDBLOCK && i_status != SERVER_SSL_ERROR_ACCEPT &&
                i_status != SERVER_SSL_ERROR_FDSET && i_status != SERVER_SSL_ERROR_NEW)
            {
                m_server.Close();
                std::cout << "Error while processing web request" << std::endl;
                return EXIT_FAILURE;
            }
            usleep(10000);
        }
        m_server.Close();
        
    }
    return EXIT_SUCCESS;
}

