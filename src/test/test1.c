#include <stdio.h>
main()
{
	syscall(401,2);
	int count = 0;
	while(count<1000000)
	{
		printf("1 		: %d\n", count);
		count++;
	}
}