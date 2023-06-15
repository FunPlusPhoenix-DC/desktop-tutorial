#include <iostream>

#include <stdio.h>

#include <thread>

#include "myTools.h"

#include "SocketTest.h"

#define DEFAULT_PORT 27015

typedef int (*pSock)(LPVOID lpParam); //type define a Function Pointer to ConnectThread's method.

typedef int (*pSockNoParam)(void); //type define a Function Pointer to listenThread.But now used.

typedef LPTHREAD_START_ROUTINE LSR; //type define type of multithread's parameter.

int main(){
    SocketTest st;

    WSAData wsaData;

        ::memset(&wsaData,0,sizeof(wsaData)); //Initial WSADATA for startup.

        int iResult = WSAStartup(MAKEWORD(2,2),&wsaData);

        if (iResult != NO_ERROR)
        {
            std::cout << "Error with opening WSAstartup--Code:" << WSAGetLastError() << std::endl;
            return 1;
        }
    SOCKET Client = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); //Create an instance for client.

    SOCKET* pConnectSocket = &Client;

    pSock pFuncConnect = SocketTest::ConnectSocket; //Create an instance for Client connect function.

    pSockNoParam pFuncListen = SocketTest::SocketListen; //Create an instance for Listen function.But not used.

    //HANDLE ListenThread = CreateThread(NULL,0,(LSR)pFuncListen,NULL,CREATE_SUSPENDED,NULL);

    HANDLE ConnectThread = CreateThread(NULL,0,(LSR)pFuncConnect,(LPVOID)&Client,CREATE_SUSPENDED,NULL);
    // Create an instance for thread start to execute Connect function.

    if (ConnectThread == NULL)
    {
        std::cout << "Failed to create thread for sending data " << std::endl;

        std::cout << "Error code is : " << GetLastError() << std::endl;

        CloseHandle(ConnectThread);

        return 1;
    }
    
    //ResumeThread(ListenThread);
        
        SocketTest::originalSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

        if (SocketTest::originalSocket == INVALID_SOCKET)
        {
            std::cout << "Error with Creating socket" << std::endl;
            return 1;
        }
        

        sockaddr_in ConnectStatus;

        sockaddr_in* lpConStatus = NULL;

        lpConStatus = (sockaddr_in*)::memset(&ConnectStatus,0,sizeof(sockaddr_in));

        ConnectStatus.sin_family = AF_INET;

        ConnectStatus.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

        ConnectStatus.sin_port = htons(6666); // Be careful of this htons.NOT htonl! 

        std::cout << "htonl:1000 ->" << htons(6666) << std::endl;

        int iSize = sizeof(ConnectStatus);

        int* piSize = &iSize;

        //std::cout << "htons: " << htons(9898) << std::endl;

        int BindResult = bind(SocketTest::originalSocket,(sockaddr*)lpConStatus,sizeof(ConnectStatus));

        if (BindResult == SOCKET_ERROR)
        {
            std::cout << "bind error" << std::endl;
            BindResult = closesocket(SocketTest::originalSocket);
            WSACleanup();
            return 1;
        }

        int listenResult = listen(SocketTest::originalSocket,SOMAXCONN);

        if (listenResult == SOCKET_ERROR)
        {
            std::cout << "listen error.Code:" << WSAGetLastError()<<std::endl;
            closesocket(SocketTest::originalSocket);
            WSACleanup();
            return 1;
        }
        
        std::cout << "Begin to listening" << std::endl;

        int iSizeofConnectStatus = 0;
        
        std::cout << "In listen Function: socket:" << SocketTest::originalSocket << std::endl;

        sockaddr_in clientStatus;   //Client's sockaddr_in.Datas are belong to connect socket.

        sockaddr_in* lpclientStatus = (sockaddr_in*)::memset(&clientStatus,0,sizeof(clientStatus)); //Initial client's datastruct

        int iSizeClientStatus = sizeof(clientStatus);

        int* lpiSizeClientStatus = &iSizeClientStatus;

        ResumeThread(ConnectThread);    //Begin ConnectThread.

        SetEvent(SocketTest::finishInitial); //NOT worked at this example.

        SOCKET acceptSocket = accept(SocketTest::originalSocket,(sockaddr*)lpclientStatus,lpiSizeClientStatus);    //Accept the connection from client.     

        if (acceptSocket == INVALID_SOCKET)
        {
            std::cout << "Accept Error. Code :" << GetLastError()<<std::endl;
            closesocket(acceptSocket);
            WSACleanup();
            return 1;
        }
        else{
            std::cout << "Accepted Waiting for connecting" << std::endl;
            std::cout << "After accepted, Socket is:" << acceptSocket << std::endl;
        }

        SetEvent(SocketTest::finishAccept); //NOT worked at this exmple.

        while (true)
        {
            char szBuffer[256] {0};
            int iRecvSize = recv(acceptSocket,szBuffer,sizeof(szBuffer),0); //Recieve data from Client.

            if (iRecvSize)
            {
                std::cout << "Recv:" << szBuffer << std::endl;
            }

            Sleep(50);    
        }
    
    std::cout << "Wait result:" << WaitForSingleObject(SocketTest::finishSend,INFINITE) << std::endl;

    for (;;)
    {
        /* code */
    }
    
    

    return 0;
}