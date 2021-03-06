#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define SERVER_SUCCESS                 0x00000
#define SERVER_ERROR                   0xaaaaa
#define SERVER_ERROR_SOCKET            0x00001
#define SERVER_ERROR_ACCEPT            0x00002
#define SERVER_ERROR_HEADER            0x00003
#define SERVER_ERROR_WOULDBLOCK        0x00004
#define SERVER_ERROR_FILE_NOT_FOUND    0x00005
#define SERVER_ERROR_GUID_STRING       "GUID_EMPTY"
#define SERVER_ERROR_OVERWRITE_STRING  "OVERWRITE"

#define SERVER_SSL_ERROR_INIT          0x90001
#define SERVER_SSL_ERROR_LOADCERT      0x90002
#define SERVER_SSL_ERROR_LOADKEY       0x90003
#define SERVER_SSL_ERROR_CHECKKEY      0x90004
#define SERVER_SSL_ERROR_ACCEPT        0x90005
#define SERVER_SSL_ERROR_FDSET         0x90006
#define SERVER_SSL_ERROR_NEW           0x90007


#define SERVER_MAX_CLIENTS             15
#define SERVER_MAX_BUFFER              65535

#define CALLBACK_FUNCT_EMPTY           0xaa00000
#define SERVER_LOG_ID_LEN              10

typedef unsigned long long ull;

#endif /* DEFINITIONS_HPP */

