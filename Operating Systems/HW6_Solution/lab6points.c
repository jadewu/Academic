#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT_NUM  8000

int main(int argc, char *argv[])
{
    int pid;
    int ii;
    int n, d;
    int fid, new_fid;
    int temp;
    struct sockaddr_in prod_addr, cons_addr, new_addr;
    socklen_t newlen=sizeof(struct sockaddr_in);
	   
    /* Obtain the sequence size */
    if (argc != 3) {
        printf("Usage: ./lab6 <n> <d>\n");      
        return -1;
    }
    n = atoi(argv[1]);
    d = atoi(argv[2]);

    /* create the child process */
    pid = fork();
	
    if(pid==0){ /* child process  - PRODUCER (CLIENT) */

        /* create a local socket */
        if((fid=socket(AF_INET,SOCK_STREAM, 0)) == -1){ = //+1 point
          printf("Error: could not create a communications socket\n");
          return -1;
        }

        /* connect to server */ //+1 point
        memset(&cons_addr, 0, sizeof(struct sockaddr_in) );          /* zero the struct */
        cons_addr.sin_family = AF_INET;                              /* IP connection */
        cons_addr.sin_port = htons(PORT_NUM);                        /* port number, short, network byte order */
        inet_aton("127.0.0.1", &cons_addr.sin_addr);                 /* connect to local host */
        if(connect(fid, (struct sockaddr*) &cons_addr, sizeof(struct sockaddr_in))){
            printf("Error: Producer (client) could not connect to server\n");
        }
        else{
            printf("Producer (client) successfuly connected to server\n");
        }
        
        for(ii=0;ii<n;ii++){
            /* wait for random time */
            usleep(1000* (rand()%1000)); //+0.5 point

            /* Write next fibonacci number to the pipe -- These are BLOCKING writes */
            temp = ii*d; //+0.5 point
            write(fid, &temp, sizeof(temp)); //+0.5 point
        }

        temp = -1;
        write(fid, &temp, sizeof(temp)); //+ 0.5 point
        /* close the write end */
        close(fid); //+0.5 point
    }
    else if(pid>0){ /* parent process - CONSUMER (SERVER) */

        /* create a local socket */
        if( (fid=socket(AF_INET,SOCK_STREAM, 0)) == -1){ //+1 point
          printf("Error: could not create a communications pipe\n");
          return -1;
        }

        /* Listen and accept producer's connection request */
        memset(&cons_addr, 0, sizeof(struct sockaddr_in) );           /* zero the struct */
        cons_addr.sin_family = AF_INET ;                              /* IP connection */
        cons_addr.sin_port = htons(PORT_NUM) ;                        /* port number, short, network byte order */
                                                                     /* Server listens to all source IP addresses */
        /* Take over the port number */ //+1 point
        if(bind(fid, (struct sockaddr*) &cons_addr, sizeof(struct sockaddr_in) )){  /* Take over the prescribed port */
           printf("Error: could not connect to producer\n");
           close(fid);
           return -1;
        }
        /* Listen and accept the connection */
        listen(fid, 1);//+0.5 point
        new_fid = accept(fid, (struct sockaddr*) &new_addr, &newlen); //+0.5 point
	   if(fid>0){
            printf("Accepted a connection from a remote socket\n");
        }
        else{
            printf("Connection accept failed\n");
        }

        /* print a header */
        printf("The sequence is ");

        while(1){
            /* read and print an entry from the shared buffer - These are BLOCKING reads */
            read(new_fid, &temp, sizeof(temp)); //+1 point
            // Wait for terminate signal from producer
            if(temp==-1)break; //+0.5 point
            else{
                printf("%d ", temp);
                fflush(stdout);
            }
        }
        printf("\n");
        
        /* wait for child to exit */
        wait(NULL); //+0.5 point
        /* close the read end */
        close(fid); //+0.5 point
    }
    else{
        printf("Error: fork() failed.. program will exit\n");
        return -1;
    }

    return 0;
}

