#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include "src/ServerSSL.hpp"

#define SSL_CERT              "./example.crt"          // location of SSL cert
#define KEY                   "./key.key"            // location of SSL key

void DeviceInfoReceived(std::string buffer)
{
    std::cout << "Device info:\n" << buffer << std::endl;
}

int main(int argc, char** argv)
{   
    ServerSSL m_server;
    
    int i_port;
    std::cout << "Podaj numer portu:";
    std::cin >> i_port;
    
    if (m_server.Init(i_port, SSL_CERT, KEY) == EXIT_SUCCESS)
    {
        m_server.attachCallback(&DeviceInfoReceived);
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

