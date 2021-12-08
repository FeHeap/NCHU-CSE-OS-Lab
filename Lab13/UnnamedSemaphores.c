#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int count;
sem_t mutex;

void* dec(void*);
void* inc(void*);

int main() {
	count = 0;
	printf("Init, count = %d\n", count);

        sem_init(&mutex, 0, 1);
        pthread_t t1, t2, t3, t4;
        pthread_create(&t1, NULL, inc, NULL);
        pthread_create(&t2, NULL, dec, NULL);
	pthread_create(&t3, NULL, inc, NULL);
        pthread_create(&t4, NULL, dec, NULL);


        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
	pthread_join(t3, NULL);
        pthread_join(t4, NULL);
        sem_destroy(&mutex);

	printf("After counting, count = %d\n", count);

        return 0;
}

void* inc(void *arg) {
	// wait
	sem_wait(&mutex);
     
	int i;
	for(i = 0; i < 25000000; i++) {
		count++;	
	}

	// signal
	sem_post(&mutex);
}

void* dec(void *arg) {
	// wait
	sem_wait(&mutex);        
	
	int i;
	for(i = 0; i < 25000000; i++) {
		count--;
	}

	// signal
	sem_post(&mutex);
}

