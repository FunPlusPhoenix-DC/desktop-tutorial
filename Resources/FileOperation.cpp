#include "FileOperation.h"

void InitAndWriteData(){
    std::cout << "*****************Begin to CreateFile*****************" << std::endl;

    bool bCreateDrect = CreateDirectoryW(L"D:\\TESTSOFT\\TestFile",NULL);

    if (bCreateDrect != ERROR_PATH_NOT_FOUND)
    {
        std::cout << "Success to create D:\\TESTSOFT\\TestFile" << std::endl;

        HANDLE hNewFile = CreateFileW(L"D:\\TESTSOFT\\TestFile\\Test.txt",
                                        GENERIC_READ | GENERIC_WRITE,
                                        0,
                                        NULL,
                                        OPEN_ALWAYS,
                                        FILE_ATTRIBUTE_NORMAL,
                                        NULL);

        if (hNewFile == INVALID_HANDLE_VALUE)
        {
            std::cout << "Failed to create File" << 
            "\n Error code is :"<< GetLastError() <<std::endl;
            goto exitCreate;
        }
        else{
            char szBuffer[256]{0};

            while (std::cin.getline(szBuffer,256))
            {
                sprintf_s(szBuffer+strlen(szBuffer),sizeof(szBuffer),"*\n");

                DWORD dwSize = 0;

                /*OVERLAPPED overLapped;

                LPOVERLAPPED lpoverLapped = (LPOVERLAPPED)::memset(&overLapped,0,sizeof(overLapped));

                HANDLE hOverlappedEvnet = CreateEvent(NULL,true,false,NULL);

                lpoverLapped->hEvent = hOverlappedEvnet;

                lpoverLapped->Offset = */

                SetFilePointer(hNewFile,0,NULL,FILE_END); //This function is used to set file pointer to the last position of file.

                bool bWriteStatus = WriteFile(hNewFile,
                                          szBuffer,
                                          strlen(szBuffer),
                                          &dwSize,
                                          NULL                                          
            );

                if (!bWriteStatus)
                {
                std::cout << "Failed to Write File" <<
                "\n Error code is :" << GetLastError() << std::endl;
                }

                ZeroMemory(szBuffer,sizeof(szBuffer));
            }           
        }
        
    }
    else
        std::cout << "Failed to create D:\\TESTSOFT\\TestFile" << std::endl;
    

    exitCreate:
    std::cout << "*****************Exit Create File test*****************" << std::endl;
}