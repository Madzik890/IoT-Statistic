#include "ServerSSL.hpp"
#include <signal.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <string>

ServerSSL::ServerSSL()
{
}

ServerSSL::~ServerSSL()
{
}

SSL_CTX *ServerSSL::initWebServerCTX(void)
{
    const SSL_METHOD *m_method;
    SSL_CTX *m_ctx;

    OpenSSL_add_all_algorithms();           /* load & register all cryptos, etc. */
    SSL_load_error_strings();               /* load all error messages */
    m_method = TLSv1_2_server_method();
    //m_method = SSLv23_server_method();

    m_ctx = SSL_CTX_new(m_method);   /* create new context from method */
    if (m_ctx == NULL)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    
    return m_ctx;
}

int ServerSSL::enableSSL()
{
    SSL_library_init();
    m_ctx = initWebServerCTX();        /* initialize SSL */
    if (m_ctx != NULL) return SERVER_SUCCESS;
    else return SERVER_SSL_ERROR_INIT;    
}

int ServerSSL::prepare(const int maxClient)
{
    int i = 1;

    this->m_ctx = NULL;
    this->i_addrlen = sizeof(this->m_address);
    if ((this->m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
    {
        return SERVER_ERROR_SOCKET;
    }
    
    this->m_address.sin_family = AF_INET;
    this->m_address.sin_addr.s_addr = INADDR_ANY;
    this->m_address.sin_port = htons(this->i_port);
    //inet_pton(AF_INET, ip, &this->m_address.sin_addr.s_addr);
    memset(this->m_address.sin_zero, '\0', sizeof this->m_address.sin_zero);
    
    if (bind(this->m_socket, (struct sockaddr *)&this->m_address, sizeof(this->m_address)) < 0)
        return SERVER_ERROR_SOCKET;
    
    if (listen(this->m_socket, maxClient) < 0)
        return SERVER_ERROR_SOCKET;
    signal(SIGCHLD, SIG_IGN);
    
    fcntl(this->m_socket, F_SETFL, O_NONBLOCK);
    setsockopt(this->m_socket, IPPROTO_TCP, TCP_NODELAY, (void *)&i, sizeof(i));
    
    return SERVER_SUCCESS;
}

const size_t ServerSSL::sslRead(SSL* clientSSL, char buffer[], const size_t length)
{
    int received, count = 0;
    fd_set fds;
    struct timeval timeout;
    bool endLoop = false;
    int err = 0;

    if (clientSSL != NULL)
    {
        while (endLoop == false)
        {
            received = SSL_read (clientSSL, buffer, length);
            if (received > 0) break;
            else
            {
                count++;
                err = SSL_get_error(clientSSL, received);
                switch (err)
                {
                    case SSL_ERROR_NONE:
                    {
                        // no real error, just try again...
                        continue;
                    }  

                    case SSL_ERROR_ZERO_RETURN:
                    {
                        // peer disconnected...
                        endLoop = true;
                        break;
                    }  

                    case SSL_ERROR_WANT_READ:
                    {
                        int sock = SSL_get_rfd(clientSSL);
                        FD_ZERO(&fds);
                        FD_SET(sock, &fds);
                        timeout.tv_sec = 5;
                        timeout.tv_usec = 0;
                        err = select(sock+1, &fds, NULL, NULL, &timeout);
                        if (err > 0) continue; // more data to read...
                        if (err == 0) endLoop = true;
                        else endLoop = true;
                        break;
                    }

                    case SSL_ERROR_WANT_WRITE:
                    {
                        // socket not writable right now, wait a few seconds and try again...
                        int sock = SSL_get_wfd(clientSSL);
                        FD_ZERO(&fds);
                        FD_SET(sock, &fds);
                        timeout.tv_sec = 4;
                        timeout.tv_usec = 0;
                        err = select(sock+1, NULL, &fds, NULL, &timeout);
                        if (err > 0) continue; // can write more data now...
                        if (err == 0) endLoop = true;
                        else endLoop = true;
                        break;
                    }

                    default:
                    {
                        endLoop = true;
                        break;
                    }
                }    

                break;
            }
        }
    }

    return received;
}

int ServerSSL::loadSSLCert(const char* cert, const char* key)
{
    /* set the local certificate from CertFile */
    if (SSL_CTX_use_certificate_file(m_ctx, cert, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return SERVER_SSL_ERROR_LOADCERT;
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if (SSL_CTX_use_PrivateKey_file(m_ctx, key, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return SERVER_SSL_ERROR_LOADKEY;
    }
    /* verify private key */
    if (!SSL_CTX_check_private_key(m_ctx))
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        return SERVER_SSL_ERROR_CHECKKEY;
    }
    
    return SERVER_SUCCESS;
}

int ServerSSL::init(unsigned int port, const char *sslCert, const char *sslKey)
{
    this->i_port = port;
    this->m_ctx = NULL;
    if (prepare() != SERVER_SUCCESS) return EXIT_FAILURE;
    enableSSL();
    loadSSLCert(sslCert, sslKey);
    return EXIT_SUCCESS;
}

int ServerSSL::waitForRequestAndProcess()
{
SSL *m_clientSSL;
int  m_clientSocket;
int res, err;

    
    if ((m_clientSocket = accept(this->m_socket, (struct sockaddr *)&this->m_address, (socklen_t*)&this->i_addrlen)) < 0)
    {
        err = errno;
        if (err == EINTR) return EINTR;
        else if (err == EWOULDBLOCK) return SERVER_ERROR_WOULDBLOCK;
        else
            return SERVER_ERROR_ACCEPT;
    }
    signal(SIGPIPE,SIG_IGN);                            //ignoring the broken pipe signal
    int i = 1;
    setsockopt(m_clientSocket, IPPROTO_TCP, TCP_NODELAY, (void *)&i, sizeof(i));
    
    if (this->m_ctx != NULL)
    {    
        m_clientSSL = SSL_new(this->m_ctx);           /* get new SSL state with context */
        if (m_clientSSL)
        {
            res = SSL_set_fd(m_clientSSL, m_clientSocket);        /* set connection socket to SSL state */
            if (res == 1)
            {
                SSL_set_options(m_clientSSL, SSL_OP_ALL);
                SSL_set_options(m_clientSSL, SSL_OP_TLS_ROLLBACK_BUG);
                
                res = SSL_accept(m_clientSSL);
                if (res == 1)
                    this->proccess(m_clientSSL);
            }
            else
                return SERVER_SSL_ERROR_NEW;   
        }
        else
           return SERVER_SSL_ERROR_NEW;              
    }
    return SERVER_SUCCESS;
}

void ServerSSL::closeClient(SSL *clientSSL)
{
    if (clientSSL != NULL)
    {        
        int client = SSL_get_fd(clientSSL);       /* get socket connection */
        SSL_shutdown(clientSSL);              /* shutdown SSL connection */
        SSL_free(clientSSL);                  /* release SSL state */
    }
}
#include <iostream>
int ServerSSL::proccess(SSL* clientSSL)
{
int i_fileLength;
long l_clientSent;
char s_buffer[SERVER_MAX_BUFFER + 1];  // +1 - terminator
char s_dataToSend[SERVER_MAX_BUFFER];
std::string s_path;
int m_file;
int i_fileStatus;
std::string WebResult;
    
    if (clientSSL != NULL)
    {
        l_clientSent = sslRead(clientSSL, s_buffer, SERVER_MAX_BUFFER);                         /* get request */
        if (l_clientSent == 1)
            l_clientSent = sslRead(clientSSL, s_buffer, SERVER_MAX_BUFFER);  /* get request */
    }
        
    if (l_clientSent <= 0)  // SSL_read result might be lower than zero in case of error
        l_clientSent = 0;     
    
    s_buffer[l_clientSent] = 0;
    
    if (l_clientSent < 5)
    {
        closeClient(clientSSL);
        return SERVER_ERROR;
    }
    
    std::cout << s_buffer << std::endl;
}

int ServerSSL::close()
{
    shutdown(this->m_socket, SHUT_RDWR);
    return SERVER_SUCCESS;
}




