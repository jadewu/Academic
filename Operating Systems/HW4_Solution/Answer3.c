#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc,char *argv[])
{
	if (argc != 3) {
		printf("Input n and d.\n");
		return 0;
	}
	
	int n = atoi(argv[1]), d = atoi(argv[2]);
	
	int fd[2], pid;
	
	/* create the pipe */
	if (pipe(fd) == 1) {
		fprintf(stderr, "Pipe failed!");
		return -1;
	}

	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Fork Failed!");
		return -11;
	}
	else if (pid == 0) {
		/* Close the read end. This is necessary*/
		close(fd[READ_END]);

		/* Prior to linux 2.6.11, pipe capacity used to be 1 page, now it is 16 pages */
		if(n*sizeof(int) < 65536){
			int buf[n];
			/* Write to the shared memory */
			for (int i = 0; i < n; i++) {
				buf[i] = i * d;
			}
			write(fd[WRITE_END], buf, n*sizeof(int));
		}else{
			printf("Pipe full. Write failed!\n");
		}
		
		/* Should be done but isn't necessary since we close it when trying to read in the parent anyway */
		close(fd[WRITE_END]); 
	}
	else {

		int child_status;
		/* Wait for the child to exit */
		waitpid(pid, &child_status, 0);
		
		int buf[n];
		
		/* Close the write end*/
		close(fd[WRITE_END]);

		if(n*sizeof(int) < 65536){
			/* Read from the pipe if the pipe was not full
				If n was causing pipe to become full, don't read buffer
				since it will only contain garbage. This was by design.
				Another approach could have been to fill the buffer up to pipe capacity */
			read(fd[READ_END], buf, n*sizeof(int));
			/* print the sequence obtained from the shared memory */
			for (int i = 0; i < n-1; i++){
	            printf("%d,", buf[i]); 
	        }
	        printf("%d\n", buf[n-1]); 
		}

        close(fd[READ_END]);
	}
	
	return 0;
}
