/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"




void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysExp(int op1, int op2)
{
  int result = 1;
  int tmp_rt;
  while (op2 > 1)
  {
    tmp_rt = op1;
    int i;
    for (i = 2; i <= op2; i *= 2)
    {
      tmp_rt *= tmp_rt;
    }
    result *= tmp_rt;
    op2 -= i / 2;
  }
  if (op2 == 1)
  {
    result *= op1;
  }
  return result;
}

int SysSub(int op1, int op2)
{
  return op1 - op2;
}




#endif /* ! __USERPROG_KSYSCALL_H__ */
