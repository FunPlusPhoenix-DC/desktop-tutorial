#include "FileAndCom.h"

#include <fstream>

bool cSerialPort::s_bExit = false;



cSerialPort::cSerialPort(/* args */):m_hListenThread(INVALID_HANDLE_VALUE)
{
    m_hComm = INVALID_HANDLE_VALUE;

    InitializeCriticalSection(&m_csCommunicationSync);
}

cSerialPort::~cSerialPort()
{
    CloseListenTread();

    ClosePort();

    DeleteCriticalSection(&m_csCommunicationSync);
}

bool cSerialPort::InitPort(UINT  portNo , UINT  baud , char  parity , UINT  databits , UINT  stopsbits , DWORD dwCommEvents ){

    char cDCBParam[50]{0};

    sprintf_s(cDCBParam,"baud=%d parity=%c data=%d stop=%d",baud,parity);
    if(!openPort(portNo)){
        std::cout << "Failed to initial port" << std::endl;
        return 0 ;
    }

    EnterCriticalSection(&this->m_csCommunicationSync);

    bool bStatus = true;

    COMMTIMEOUTS ComTimeouts;

    LPCOMMTIMEOUTS lpComTimeouts = NULL;

    lpComTimeouts = (LPCOMMTIMEOUTS)::memset(&ComTimeouts,0,sizeof(ComTimeouts));

    if (bStatus)
    {
        bStatus = SetCommTimeouts(m_hComm,lpComTimeouts);
    }
    
    DCB dcb;

    DWORD dwNum = MultiByteToWideChar(CP_ACP,0,cDCBParam,-1,NULL,0);

    //wchar_t* pwNum = new wchar_t(dwNum); //记得释放

    const WCHAR* pwdwNum = new WCHAR(dwNum);

    if (bStatus)
    {
        bStatus = GetCommState(m_hComm,&dcb) && BuildCommDCBW(pwdwNum,&dcb);

        dcb.fRtsControl = RTS_CONTROL_ENABLE;

        delete[] pwdwNum;

        pwdwNum = NULL;

        if (bStatus)
        {
            bStatus = SetCommState(m_hComm,&dcb);

            PurgeComm(m_hComm,PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_TXABORT);

            LeaveCriticalSection(&this->m_csCommunicationSync);
        }   
    }
}

bool cSerialPort::InitPort(UINT  portNo, const LPDCB& plDCB){
    if (openPort(portNo))
    {
        return false;
    }
    
    EnterCriticalSection(&this->m_csCommunicationSync);

    if (!SetCommState(m_hComm,plDCB))
    {
        return false;
    }
    
    PurgeComm(m_hComm,PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_TXABORT);

    LeaveCriticalSection(&this->m_csCommunicationSync);

    return true;
}

void cSerialPort::ClosePort(){
    if (m_hComm!=INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hComm);

        m_hComm = INVALID_HANDLE_VALUE;
    }
    
}

bool cSerialPort::openPort(UINT  portNo){
    EnterCriticalSection(&this->m_csCommunicationSync);

    char szPort[50]{0};

    sprintf_s(szPort,"COM%d",portNo);

    m_hComm = CreateFileA(szPort,
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        0);

    if(m_hComm == INVALID_HANDLE_VALUE){
        LeaveCriticalSection(&this->m_csCommunicationSync);

        std::cout << "Failed to open Port erro code is :" <<GetLastError()<< std::endl;

        return false;
    }

}

bool cSerialPort::OpenListenThread(){
    if (m_hListenThread == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    else{
        s_bExit = false;

        UINT threadID = 0;

        m_hListenThread = (HANDLE)_beginthreadex(nullptr,
                                                0,
                                                ListenThread,
                                                this,
                                                0,
                                                &threadID);

        if(!m_hListenThread){
            std::cout << "__beginThreadEx error code is : " << GetLastError() << std::endl;

            return false;
        }

        if (!SetThreadPriority(m_hListenThread,THREAD_PRIORITY_ABOVE_NORMAL))
        {
            /* code */
            std::cout << "SetHtreadPriority error code is : " << GetLastError() << std::endl;

            return false;
        }

        return true;
        
    }
    
}

bool cSerialPort::CloseListenTread(){
    if (m_hComm != INVALID_HANDLE_VALUE)
    {
        s_bExit = true;

        CloseHandle(m_hComm);

        m_hComm = INVALID_HANDLE_VALUE;
    }

    return true;
    
}

UINT cSerialPort::GetBytesInCOM(){
    DWORD dwErrorCode = 0;

    COMSTAT csComstat;

    LPCOMSTAT lpcsComstat = (LPCOMSTAT)::memset(&csComstat,0,sizeof(csComstat));

    UINT BytesInQueue = 0;

    if (ClearCommError(m_hComm,&dwErrorCode,lpcsComstat))
    {
        BytesInQueue = lpcsComstat->cbInQue;
    }

    return BytesInQueue;
    
}

UINT WINAPI cSerialPort::ListenThread(LPVOID pParam){
    cSerialPort* pcSerialPort = reinterpret_cast<cSerialPort*>(pParam); //获得本类指针

    while (!pcSerialPort->s_bExit)
    {
        UINT readBytes = pcSerialPort->GetBytesInCOM();

        if(!readBytes){
            Sleep(SLEEP_TIME_INTERVAL);

            continue;
        }

        char cRecvd = 0x00;

        do
        {
            if(pcSerialPort->ReadChar(cRecvd)){
                std::cout << "Received:" << cRecvd << std::endl;

                continue;
            }
        } while (--readBytes);
        
    }
    
    return 0;
}

bool cSerialPort::ReadChar(char& cRecved){
    bool bResult = false;

    DWORD readBytes = 0;

    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        std::cout << "In readChar func m_hComm is invalid \n" <<
                     "error code is :" << GetLastError() << std::endl;

        return 0;
    }

    EnterCriticalSection(&this->m_csCommunicationSync);

    bResult = ReadFile(m_hComm,&cRecved,1,&readBytes,NULL);

    if (!bResult)
    {
        std::cout << "ReadFile error code is " << GetLastError() << std::endl;

        PurgeComm(m_hComm,PURGE_RXCLEAR|PURGE_RXABORT);

        LeaveCriticalSection(&this->m_csCommunicationSync);

        return false;
    }
    
    LeaveCriticalSection(&this->m_csCommunicationSync);

    return (readBytes == 1);
}

bool cSerialPort::WriteData(unsigned char* pData, unsigned int length){
    bool bResult = false;

    DWORD sendBytes = 0;

    if(m_hComm == INVALID_HANDLE_VALUE){
        std::cout << "Write data error is " << GetLastError() << std::endl;

        return false;
    }

    EnterCriticalSection(&this->m_csCommunicationSync);

    bResult = WriteFile(m_hComm,pData,length,&sendBytes,NULL);

    if(!bResult){
        std::cout << "Write file error :" << GetLastError() << std::endl;

        PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_TXABORT);

        LeaveCriticalSection(&this->m_csCommunicationSync);

        return false;
    }

    LeaveCriticalSection(&this->m_csCommunicationSync);

    return true;
}