#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	// Print Hello World messge
	printf("Hello, world! \n");

	// Print a random number between 0 to 99
	int n = random() % 100;
	printf("%d \n", n);

	// Print a blank line
	printf("\n");
	
	return 0;
}