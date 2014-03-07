#include <stdio.h>

main()
{
	syscall(401,20);
	int count = 0;
	while(count<1000000)
	{
		printf("0		: %d\n", count);
		count++;
	}
}
