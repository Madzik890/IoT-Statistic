#ifndef SSL_HPP
#define SSL_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>
#include "Definitions.hpp"

class ServerSSL
{
protected:
    void caller(std::string buffer);
    SSL_CTX *initServerCTX(void);
    int enableSSL();
    int prepare(const int maxClient = SERVER_MAX_CLIENTS);
    const size_t sslRead(SSL *clientSSL, char buffer[SERVER_MAX_BUFFER], const size_t length);
    int loadSSLCert(const char *cert, const char *key);
public: 
    ServerSSL();
    ~ServerSSL();
    
    int attachCallback(void (*serverCallBack)(std::string buffer));
    
    int Init(unsigned int port, const char *sslCert, const char *sslKey);
    int WaitForRequestAndProcess();
    int Close();
private:
    struct sockaddr_in m_address;
    SSL_CTX            *m_ctx;
    int                m_socket;
    int                i_addrlen;
    int                i_port;
    void (*serverCallBack)(std::string buffer);
    
    inline void closeClient(SSL *clientSSL);
    int proccess(SSL *clientSSL);
};

#endif /* SSL_HPP */

