#include "syscall.h"

int
main()
{
  int result;
  
  result = Exp(2, 10);

  Halt();
  /* not reached */
}
