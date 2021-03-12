#include <cstdlib>
#include <unistd.h>
#include "src/ServerSSL.hpp"

#define PORT                  8000                      // default SSL port
#define SSL_CERT              "./example.crt"          // location of SSL cert
#define KEY                   "./key.key"            // location of SSL key

int main(int argc, char** argv)
{   
    ServerSSL m_server;
    
    if (m_server.Init(8000, SSL_CERT, KEY) == EXIT_SUCCESS)
    {
        while(1)    
        {
            int i_status = m_server.WaitForRequestAndProcess();
            if (i_status != SERVER_SUCCESS && i_status != SERVER_ERROR_WOULDBLOCK && i_status != SERVER_SSL_ERROR_ACCEPT &&
                i_status != SERVER_SSL_ERROR_FDSET && i_status != SERVER_SSL_ERROR_NEW)
            {
                m_server.Close();
                printf("Error while processing web request");
                return EXIT_FAILURE;
            }
            usleep(10000);
        }
        m_server.Close();
        
    }
    return EXIT_SUCCESS;
}

