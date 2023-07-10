#include <iostream>

#include <stdio.h>

#include <thread>

#include "myTools.h"

#include "SocketTest.h"

#include "FileAndCom.h"

#include "FileOperation.h"

#include "dllfunc.h"

#include "MathLibrary.h"

#include <regex> //Test regex output

#define DEFAULT_PORT 27015

#define UNICODE

typedef int (*pSock)(LPVOID lpParam); //type define a Function Pointer to ConnectThread's method.

typedef int (*pSockNoParam)(void); //type define a Function Pointer to listenThread.But now used.

typedef LPTHREAD_START_ROUTINE LSR; //type define type of multithread's parameter.

typedef struct EuropeEngineer
{
    std::string luke;

    std::string houvi;

    std::string danny;
}EE,*lpEE;

template<typename T>
class TestTransmit
{
private:
    /* data */
public:
    TestTransmit(/* args */);
    ~TestTransmit();

    TestTransmit(T&A,T B);

    T m_boolA;

    T m_boolB;
};

template <class T> 
TestTransmit<T>::TestTransmit(/* args */)
{
}

template <class T>
TestTransmit<T>::~TestTransmit()
{
}

template <typename T>
TestTransmit<T>::TestTransmit(T&A,T B):m_boolA(A),m_boolB(B){
    std::cout << "Origin A:" << A << std::endl;

    std::cout << "Origin B:" << B << std::endl;

    std::cout << "***********************" << std::endl;

    A = 0;

    B = 0;
}



int main(){
    SocketTest st;

    std::cout << "SendEvent:" << SocketTest::finishSend << std::endl;

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

    if (ConnectThread == INVALID_HANDLE_VALUE)
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

        //SetEvent(SocketTest::finishAccept); //NOT worked at this exmple.

        while (true)
        {
            char szBuffer[256] {0};
            int iRecvSize = recv(acceptSocket,szBuffer,sizeof(szBuffer),0); //Recieve data from Client.

            if (iRecvSize)
            {
                std::cout << "Recv:" << szBuffer << std::endl;
            }
            else{
                break;
            }

            ::memset(szBuffer,0,sizeof(szBuffer));

            Sleep(50);    
        }
    
    if(WaitForSingleObject(SocketTest::finishSend,INFINITE) == WAIT_OBJECT_0);

    std::cout << "SendEvent:" << SocketTest::finishSend << std::endl;

    while (!SocketTest::finishAll) //Block the functions instead of using WaitForSingleObject.
    {
        /* code */

        std::cout << "Waiting send data thread end" << std::endl;
    }

    std::cout << "*****************Begin to test cSerialPort*****************" << std::endl;

    /*cSerialPort cSP;  

    DCB TestPortDCB;

    LPDCB lpTestPortDCB = (LPDCB)::memset(&TestPortDCB,0,sizeof(TestPortDCB));

    if(!cSP.InitPort(3,9600,'N',8,1,EV_RXCHAR)){
        std::cout << "Port initial failed\n error code is :" << GetLastError() << std::endl; 
    }
    else
        std::cout << "Success to initial port" << std::endl;

    if(!cSP.OpenListenThread()){
        std::cout << "ListenThread failed to open\n error code is :" << GetLastError() << std::endl;
    }
    else
        std::cout << "Success to open listen thread" << std::endl;

    unsigned char* data = new unsigned char[10];
	data[0] = 0x54;  // Head_H
	data[1] = 0x80;  // Head_L
	data[2] = 0x01;  // 01 pics; 02 brightness; 03 focal length
	data[3] = 0x00;  // pics number  

	//for (i = 4; i < 10; i++)
	//	data[i] = 0x00;    // Initial value is 0

	cSP.WriteData(data, 10);

	for (int i = 0; i < 18; i++)
	{
		data[3]++;
		std::cout << i << " data[3]=" << data[3] << std::endl;
		//cout << "data[" << i << "]=" << data[i] << endl;
		cSP.WriteData(data, 10);
		Sleep(0.5 * 1000);
	}
	// 17 pictures are projectored
 
	int temp;
	std::cin >> temp;

    operator delete[](data);

    data = NULL;*/
    // TestTransmit<int> tplTest(iNum,iNum2);  <- this is the template initialize.
    //InitAndWriteData();

    std::cout << "CloseHandle:" << CloseHandle(SocketTest::finishAccept) << std::endl;

    std::cout << "CloseHandle:" << CloseHandle(SocketTest::finishSend) << std::endl;

    std::cout << "*****************Begin to test regex*****************" << std::endl;

    std::string str = "\\*Error:";

    char str_com[256]{0};

    std::cin.getline(str_com,256);

    std::cout << str_com << std::endl;

    std::regex str_reg(str);

    std::cout << "after regex str_reg == " << 
    std::regex_search(str_com,str_reg) << std::endl;

    if(std::regex_search(str_com,str_reg))
        std::cout << "Search:" << str_com << std::endl;

    /*HMODULE hmdllTest = LoadLibraryW(L"D:\\TESTSOFT\\Linkers\\dllMain.dll");

    if(hmdllTest != NULL){                                                //use function from dll through GetProcAddress
        FARPROC farprocDll = GetProcAddress(hmdllTest,"Test");

        if (farprocDll != NULL)
        {
            std::cout << "pointer is :" << (void*)farprocDll << std::endl;

            std::cout << farprocDll() << std::endl;
        }
        else
            std::cout << "farprocDll is error address\n" <<
            "errorcode is " << GetLastError() << std::endl;
        
    }
    else
        std::cout << "hmdllTest (HMODULE) is NULL" << std::endl;*/

    /*fibonacci_init(1, 1);                                             //dll built by vs 2022;
    // Write out the sequence values until overflow.
    do {
        std::cout << fibonacci_index() << ": "
            << fibonacci_current() << std::endl;
    } while (fibonacci_next());
    // Report count of values written before overflow.
    std::cout << fibonacci_index() + 1 <<
        " Fibonacci sequence values fit in an " <<
        "unsigned 64-bit integer." << std::endl;*/

    dllfunc dll;

    dll.Test();

    system("pause");

    return 0;
}