#ifndef _DLLFUNC_H_

#define _DLLFUNC_H_

#include <iostream>

#include <Windows.h>

typedef int*(*ipFunc)(void*);

typedef HANDLE (*hpFunc)(void*);

/*#ifdef __cplusplus 
extern "C" { 
#endif

//一段代码

#ifdef __cplusplus 
} 
#endif 

#define _cplusplus*/

#define DLLFUN_EXPORTS

#ifdef DLLFUN_IMPORTS

#define DLLFUN_API __declspec(dllimport)

#else

#define DLLFUN_API __declspec(dllexport)
                                        //For linux .Keep it blank In WIN32 environment. 

#endif

class DLLFUN_API dllfunc
{
private:
  /* data */
public:
  dllfunc(/* args */);
  ~dllfunc();

  int Min();

  int Max();

  int memberSize();

  void Test(void);

  int regedit;
};


#endif