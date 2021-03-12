#ifndef CLIENTSSL_HPP
#define CLIENTSSL_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "../Definitions.hpp"
#include <string>

class ClientSSL
{
protected:
    SSL_CTX *initClientCTX(void);
    int enableSSL();
    int prepare();
    const size_t sslRead(SSL *clientSSL, char buffer[CLIENT_MAX_BUFFER], const size_t length);
    int openConnection();
public: 
    ClientSSL(char* ip, const int port);
    ~ClientSSL();
    
    int Init();
    int SendMessage(std::string message);
    int Close();
    
    const int GetPort();
    const char *GetIp();
    void SetPort(const int port);
    void SetIp(const char *ip);
private:
    struct sockaddr_in m_address;
    SSL_CTX            *m_ctx;
    int                m_socket;
    int                i_addrlen;
    int                i_port;
    std::string        s_ip;
};

#endif /* CLIENTSSL_HPP */

