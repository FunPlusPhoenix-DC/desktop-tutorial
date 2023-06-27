#ifndef _FILEANDCOM_H_

#define _FILEANDCOM_H_

#include <stdio.h>

#include <iostream>

#include <stdlib.h>

#include <string>

#include <fileapi.h> //CreateFileW basement.

#include <windows.h> //CRB_9600 definition.  winbase.h have some error so using windows.h instead;

#include <process.h>

#include <stringapiset.h>

#define INITIAL_ALL_PORT 0X01

#define CLOSE_ALL_PORT 0x02

#define SLEEP_TIME_INTERVAL 5

class cSerialPort
{
private:
    /* data */
    bool openPort(UINT  portNo);

    void ClosePort();

    static UINT WINAPI ListenThread(LPVOID pParam);

    HANDLE  m_hComm;

    volatile HANDLE    m_hListenThread;

    CRITICAL_SECTION   m_csCommunicationSync;

public:
    cSerialPort(/* args */);
    ~cSerialPort();

    //Function begin

    static bool s_bExit;

    bool InitPort(UINT  portNo , UINT  baud , char  parity , UINT  databits , UINT  stopsbits , DWORD dwCommEvents );

    bool InitPort(UINT  portNo, const LPDCB& plDCB);

    bool OpenListenThread();

    bool CloseListenTread();

    bool WriteData(unsigned char* pData, unsigned int length);

    UINT GetBytesInCOM();

    bool ReadChar(char &cRecved);

    bool ReadData(char& cRecved,unsigned length);
};


#endif