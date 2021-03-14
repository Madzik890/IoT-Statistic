#include "ClientSSL.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <resolv.h>

ClientSSL::ClientSSL(const char *ip, const int port)
:s_ip(ip), i_port(port)
{
    
}

ClientSSL::~ClientSSL()
{
    i_port = 0;
    this->Close();
}

SSL_CTX *ClientSSL::initClientCTX()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    method = TLSv1_2_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

const size_t ClientSSL::sslRead(SSL* clientSSL, char buffer[], const size_t length)
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

const int ClientSSL::sendMessage(std::string message, std::string *received)
{
    SSL *m_ssl;
    char s_buffer[CLIENT_MAX_BUFFER + 1];  // +1 - terminator;
    long l_clientSent;
    
    m_socket = openConnection();
    m_ssl = SSL_new(m_ctx);      /* create new SSL connection state */
    SSL_set_fd(m_ssl, m_socket);    /* attach the socket descriptor */
    if ( SSL_connect(m_ssl) == CLIENT_ERROR_CONNECT)   /* perform the connection */
    {
        ERR_print_errors_fp(stderr);
        return CLIENT_ERROR_CONNECT;
    }
    else
    {  
        SSL_write(m_ssl, message.c_str(), message.size());   /* encrypt & send message */
        
        l_clientSent = sslRead(m_ssl, s_buffer, CLIENT_MAX_BUFFER);                         /* get request */
        if (l_clientSent == 1)
            l_clientSent = sslRead(m_ssl, s_buffer, CLIENT_MAX_BUFFER);  
        if(l_clientSent > 0 && received != nullptr)
        {
            s_buffer[l_clientSent] = 0;
            *received = std::string(s_buffer);
        }
        SSL_free(m_ssl);        /* release connection state */
    }
    close(m_socket);         /* close socket */
    
    return CLIENT_SUCCESS;
}

int ClientSSL::openConnection()
{
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;

    if ( (host = gethostbyname(s_ip.c_str())) == NULL )
        return CLIENT_ERROR_HOSTNAME;
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(i_port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
       close(sd);
       return CLIENT_ERROR_CONNECT;
    }
    return sd;
}

int ClientSSL::Init()
{
    SSL_library_init();
    m_ctx = initClientCTX();
}

int ClientSSL::SendInfo(InfoManager& manager)
{
    return sendMessage(manager.GetXML());
}

int ClientSSL::SendLog(LogInfo& log)
{
    if(log.GetLogSentID().empty())
    {
        int i_result;
        std::string s_logID;
        i_result = sendMessage(log.ToString(), &s_logID);
        if(!s_logID.empty())
        {
            if(s_logID.compare(CLIENT_ERROR_OVERWRITE_STRING) == 0)//klient przesłał kiedyś LOG
            {
                log.SetLogSentID(CLIENT_ERROR_OVERWRITE_STRING);
                return CLIENT_ERROR_OVERWRITE;
            }
            else
            if(s_logID.compare(CLIENT_ERROR_GUID_STRING) == 0)//klient nie przesłał GUID
            {
                return CLIENT_ERROR_GUID;
            }
            else                                 //klient pierwszy raz przesłał logi
            {
                log.SetLogSentID(s_logID);
                return CLIENT_SUCCESS;
            }
                
        }
        return i_result;
    }
    else
        return CLIENT_ERROR_OVERWRITE;
}

int ClientSSL::Close()
{
    shutdown(this->m_socket, SHUT_RDWR);
    SSL_CTX_free(m_ctx);        /* release context */
}

const int ClientSSL::GetPort()
{
    return i_port;
}

const char *ClientSSL::GetIp()
{
    return s_ip.c_str();
}

void ClientSSL::SetPort(const int port)
{
    i_port = port;
}

void ClientSSL::SetIp(const char* ip)
{
    s_ip = ip;
}
