#ifndef SSL_HPP
#define SSL_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "Definitions.hpp"

class ServerSSL
{
protected:
    SSL_CTX *initWebServerCTX(void);
    int enableSSL();
    int prepare(const int maxClient = SERVER_MAX_CLIENTS);
    const size_t sslRead(SSL *clientSSL, char buffer[SERVER_MAX_BUFFER], const size_t length);
    int loadSSLCert(const char *cert, const char *key);
public: 
    ServerSSL();
    ~ServerSSL();
    
    int init(unsigned int port, const char *sslCert, const char *sslKey);
    int waitForRequestAndProcess();
    int close();
private:
    struct sockaddr_in m_address;
    SSL_CTX            *m_ctx;
    int                m_socket;
    int                i_addrlen;
    int                i_port;
    
    inline void closeClient(SSL *clientSSL);
    int proccess(SSL *clientSSL);
};

#endif /* SSL_HPP */

