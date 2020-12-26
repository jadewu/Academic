#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <math.h>
#include <semaphore.h>

pthread_t tid[4]; 
double x, y, d, s; 
// pthread_mutex_t lock;
sem_t mutex;
int worker=0, n = 0;
int NUM_POINTS=1000000;

void* add_points(void* arg) 
{ 
	unsigned long i = 0; 
	worker += 1;
    int curr = worker; 

	/* generate random point */
	srand((unsigned)time(0));

    for (i=0; i <NUM_POINTS; i++) { 
    	/* generate random point */
        x = (double)rand()/RAND_MAX*2.0-1.0;
        y = (double)rand()/RAND_MAX*2.0-1.0;
        //printf("x=%f, y=%f\n", x, y);

        /* calculate distance and increase n */
        d = sqrt(pow(x, 2)+pow(y, 2));
		if (d <= 1){
      		sem_wait(&mutex); 
			n += 1;
      		sem_post(&mutex); 
		}
		// printf("d=%f\n", d);
    }

	printf("\n Worker %d has finished, n = %d\n", curr, n); 

	return NULL; 
} 

int main(void) 
{ 
	int i = 0; 
	sem_init(&mutex, 0, 1); 

	while (i < 4) { 
    	pthread_create(&(tid[i]), NULL, &add_points, NULL);

		i++; 
	} 

	pthread_join(tid[0], NULL); 
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
	pthread_join(tid[3], NULL);
  
    /* print result */
	s = ((double)n/(double)NUM_POINTS);

	printf("\n There are %d points in the circle\n", n);
    printf("\n Area of the circle is %lf\n", s);

	sem_destroy(&mutex); 

	return 0; 
} 
