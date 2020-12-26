#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

int main(int argc,char *argv[])
{
	if (argc < 3) {
		printf("Input n and d\n");
		return 0;
	}
	
	/* Getting the values for n and d */
	int n=atoi(argv[1]), d=atoi(argv[2]);
	
	/* Size of shared memory */
	const int SIZE = n*sizeof(int);
	
	/* Pointer to shared memory */
	int *ptr = (int*)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	
	int pid = fork(), state;
	
	if (pid < 0) {
		fprintf(stderr, "Fork Failed");
		return -1;
	}else if (pid == 0) {
		int count = 0;	

		/* Write to shared memory */
		while(count<(n)){
			ptr[count] = count * d;
			count++;
		}
	}else {
		/* Wait for the child to exit */
		wait(&state);
		
		/* Print the integers from shared memory */
		for(int i=0;i<n;++i) {
        	printf("%d ",ptr[i]);
        }
        printf("\n");
	}
	
	return 0;
}
