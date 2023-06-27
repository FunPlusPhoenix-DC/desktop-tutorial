#ifndef _SOCKETTEST_H_

#define _SOCKETTEST_H_


#include <winsock2.h>

#include <windows.h>

#include <ws2tcpip.h>

#include <stdio.h>

#include <iostream>

#include <synchapi.h>


#pragma comment(lib,"ws2_32.lib")

#define libExport __declspec(dllexport);

#define DEFAULT_BUFLEN 512

#define DEFAULT_PORT 27015


/*开始从WS_32.LIB中导入需要用到的函数*/
/*
libExport int WSAStartup(
        WORD      wVersionRequired,
     LPWSADATA lpWSAData
);

libExport int WSACleanup();

libExport SOCKET WSAAPI socket(
    int af,
    int type,
    int protocol
);

libExport unsigned long WSAAPI inet_addr(
  const char *cp
);

libExport u_short WSAAPI htons(
    u_short hostshort
);

libExport int bind(
    SOCKET         s,
       const sockaddr *addr,
    int            namelen
);

libExport int WSAAPI closesocket(
    SOCKET s
);

libExport int WSAAPI listen(
    SOCKET s,
    int    backlog
);

libExport SOCKET WSAAPI accept(
         SOCKET   s,
         sockaddr *addr,
         int      *addrlen
);

libExport int WSAAPI recv(
     SOCKET s,
     char   *buf,
     int    len,
     int    flags
);

libExport int WSAAPI connect(
    SOCKET         s,
    const sockaddr *name,
    int            namelen
);

libExport int WSAAPI send(
    SOCKET     s,
    const char *buf,
    int        len,
    int        flags
);
*/

class SocketTest
{
private:
    /* data */
public:
    SocketTest(/* args */);
    ~SocketTest();

    SocketTest(unsigned int& uiIp,int& iPORT,WORD& wVersion);

    typedef struct NetworkStatus{
        unsigned int IP;
        int PORT;
    }Ns,*lpNs;

    WORD wVersionRequested;

    static bool finishAll;

    WSAData wsaData;

    NetworkStatus Nst;

    static SOCKET originalSocket;

    int SocketStart();

    int static SocketListen();

    int SocketClose();

    int static ConnectSocket(LPVOID lpParam);

    static HANDLE finishSend;

    static HANDLE finishInitial;

    static HANDLE finishAccept;
};


#endif