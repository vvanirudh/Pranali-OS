#include <stdio.h>

int main()
{
  int a;
  a = 32123;	
  syscall(402, 0, 4, &a, 100, 0);
  printf("program unblocked\n");
  return 0;
}
