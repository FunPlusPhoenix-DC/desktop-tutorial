#ifndef _DLLFUNC_H_

#define _DLLFUNC_H_

#include <iostream>

#include <Windows.h>

typedef int*(*ipFunc)(void*);

typedef HANDLE (*hpFunc)(void*);

#define _cplusplus

#define DLLFUN_IMPORTS

#ifdef DLLFUN_IMPORTS

#define DLLFUN_API __declspec(dllimport)

#else

#define DLLFUN_API __declspec(dllexport)
                                        //For linux .Keep it blank In WIN32 environment. 

#endif


  extern "C" DLLFUN_API int Min();

  extern "C" DLLFUN_API int Max();

  extern "C" DLLFUN_API int memberSize();


extern "C" DLLFUN_API void Test(void);

#endif