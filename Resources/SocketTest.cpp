#include "SocketTest.h"



HANDLE SocketTest::finishSend = CreateEventW(NULL,true,false,NULL);

HANDLE SocketTest::finishInitial = CreateEventW(NULL,true,false,NULL);

HANDLE SocketTest::finishAccept = CreateEventW(NULL,true,false,NULL);

SOCKET SocketTest::originalSocket = INVALID_SOCKET;

SocketTest::SocketTest()
{
}

SocketTest::~SocketTest()
{
}

SocketTest::SocketTest(unsigned int& uiIp,int& iPORT,WORD& wVersion):wVersionRequested(wVersion){

    Nst.IP = uiIp;

    Nst.PORT = iPORT;

    

}

int SocketTest::SocketStart(){
    /*int err = 0;

    err = WSAStartup(MAKEWORD(2,2),&wsaData);

    if (err != NO_ERROR)
    {
        char szError[64]{0};

        sprintf(szError,"WSAstartup failed with error:%d\n",err);

        std::cout << szError << std::endl;

        return 1;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        std::cout << "could not find a usable version of Winsock.dll\n" << std::endl;

        WSACleanup();

        return 1;
    }
    
    else{
        std::cout << "Has found the suitable Winsock2.dll\n Operate next" << std::endl;

        SOCKET ConnectSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        
        int err = WSAGetLastError();

        if (ConnectSocket == INVALID_SOCKET)
        {
            wprintf(L"socket failed to open with error : %d",err);

            WSACleanup();

            return 1;
        }*/
        

    //WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup function failed with error: %d\n", iResult);
        return 1;
    }
    //----------------------
    // Create a SOCKET for connecting to server
    SOCKET ConnectSocket;
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        wprintf(L"socket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    originalSocket = ConnectSocket;
}

int SocketTest::SocketClose()
{
    WSACleanup();
    return 0;
}

int SocketTest::SocketListen(){ //Not used.Becasue of the abnormal WaitForSingleObject.
        WSAData wsaData;

        ::memset(&wsaData,0,sizeof(wsaData));

        int iResult = WSAStartup(MAKEWORD(2,2),&wsaData);

        if (iResult != NO_ERROR)
        {
            std::cout << "Error with opening WSAstartup--Code:" << WSAGetLastError() << std::endl;
            return 1;
        }
        
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

        ConnectStatus.sin_port = htonl(5555);

        int iSize = sizeof(ConnectStatus);

        int* piSize = &iSize;

        //std::cout << "htons: " << htons(9898) << std::endl;

        int BindResult = bind(SocketTest::originalSocket,(sockaddr*)lpConStatus,sizeof(ConnectStatus));

        if (BindResult == SOCKET_ERROR)
        {
            std::cout << "bind error" << std::endl;
            BindResult = closesocket(originalSocket);
            WSACleanup();
            return 1;
        }

        int listenResult = listen(originalSocket,SOMAXCONN);

        if (listenResult == SOCKET_ERROR)
        {
            std::cout << "listen error.Code:" << WSAGetLastError()<<std::endl;
            closesocket(SocketTest::originalSocket);
            WSACleanup();
            return 1;
        }
        
        std::cout << "Begin to listening" << std::endl;

        int iSizeofConnectStatus = 0;

        SetEvent(finishInitial);
        
        std::cout << "In listen Function: socket:" << originalSocket << std::endl;

        sockaddr_in clientStatus;

        sockaddr_in* lpclientStatus = (sockaddr_in*)::memset(&clientStatus,0,sizeof(clientStatus));

        int iSizeClientStatus = sizeof(clientStatus);

        int* lpiSizeClientStatus = &iSizeClientStatus;

        SOCKET acceptSocket = accept(originalSocket,(sockaddr*)lpclientStatus,lpiSizeClientStatus); 

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

        SetEvent(finishAccept);

        while (true)
        {
            char szBuffer[256] {0};
            int iRecvSize = recv(acceptSocket,szBuffer,sizeof(szBuffer),0);

            if (iRecvSize)
            {
                std::cout << "Recv:" << szBuffer << std::endl;
            }

            Sleep(50);    
        }
                

        WSACleanup();
    //}
        return 0;
}

int SocketTest::ConnectSocket(LPVOID lpParam)
{

    WSAData wsaData;        //Server and client need this WSAStartup both.

        ::memset(&wsaData,0,sizeof(wsaData));

        int iResult = WSAStartup(MAKEWORD(2,2),&wsaData);

        if (iResult != NO_ERROR)
        {
            std::cout << "Error with opening WSAstartup--Code:" << WSAGetLastError() << std::endl;
            return 1;
        }

    SOCKET socket = *(SOCKET*)lpParam; //Need to change LPVOID to socket* value,then get value from this address.

    std::cout << "In ConnectSocket &socket:" << &socket << std::endl;

    WaitForSingleObject(finishInitial,INFINITE); //NOT worked.

    if(socket != INVALID_SOCKET){
        sockaddr_in ConnectStatus;

        ConnectStatus.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

        ConnectStatus.sin_family =AF_INET;

        ConnectStatus.sin_port = htons(6666); //as same as Server like.

        //WaitForSingleObject(finishAccept,INFINITE);

        int iResult = 0;

        while (true)
        {
           iResult = connect(socket,(sockaddr*)&ConnectStatus,sizeof(ConnectStatus)); //Connect to server.

           if (!iResult)
           {
            break;
           }
           else{
            std::cout << "trying to connect" << std::endl;
           }
           Sleep(500);
        }
        

        std::cout << "&socket:" << &socket << std::endl;

        if (iResult != SOCKET_ERROR)
        {
            char szBuffer[256]{0}; //Send buffer.

            sprintf_s(szBuffer,"Here's from Terence");

            int __count = 0;

            while (__count < 100)
            {
                int iSendResult = send(socket,szBuffer,sizeof(szBuffer),0);

                __count++;

                Sleep(100);

                if (__count > 5)
                {
                    /* code */
                    
                    goto exit;
                }
                
            }            
            
        }
        else{
            std::cout << "connect error return to 0 state" << std::endl;

            goto exit;  //If failed to check the Connect socket,jump to exit.
        }
        
    }
    else
        std::cout << "socket is invalid in ConnectSocket Func-> Socket:" << socket << std::endl;
    exit:
    closesocket(socket);

    SetEvent(finishSend);

    WSACleanup();

    return 0;
}
