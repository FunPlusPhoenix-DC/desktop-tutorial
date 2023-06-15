#include "myTools.h"

/*#if!def _MYTOOLS_H_

#define _MYTOOLS_H_

#endif*/

#include <iostream>

void ChangeNum_Pointer(int *pNum)
{
}

void ChangeNum_Reference(int& Num){

}

void ChangeNum_PR(int*& PRNum){
    *PRNum = 20;
    std::cout << "Num has been changed!" << std::endl;
    PRNum = (int*)0x000000001;
    std::cout << "pNum has been changed!" << std::endl;
}