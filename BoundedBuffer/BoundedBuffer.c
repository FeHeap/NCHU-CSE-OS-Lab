#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>

#define BIG_BUFFER_SIZE 1024
#define DIVIDE_NUM 4

int maximum = 0;
int minimum = INT_MAX;

sem_t max_buffer_mutex;
sem_t max_buffer_full;
sem_t min_buffer_mutex;
sem_t min_buffer_full;

int big_buffer[BIG_BUFFER_SIZE];
int max_buffer[DIVIDE_NUM];
int producer_max_put = 0;
int min_buffer[DIVIDE_NUM];
int producer_min_put = 0;

void *producer(void *args) {
	int max = 0, min = INT_MAX;
	int range = BIG_BUFFER_SIZE / DIVIDE_NUM;
	int start = *((int*)args) * range;
	int end = (*((int*)args) == DIVIDE_NUM-1)? BIG_BUFFER_SIZE : start + range;
	int i;
	for(i = start; i < end; i++) {
		if(big_buffer[i] > max) {
			max = big_buffer[i];
		}
		if(big_buffer[i] < min) {
			min = big_buffer[i];
		}
	}
	printf("Producer: Temporary max=%d and min=%d\n", max, min);
	
	sem_wait(&max_buffer_mutex);
	max_buffer[producer_max_put] = max;
	printf("Producer: Put %d into max_buffer at index %d\n", max, producer_max_put++);
	sem_post(&max_buffer_mutex);
	sem_post(&max_buffer_full);
	
	sem_wait(&min_buffer_mutex);
	min_buffer[producer_min_put] = min;
	printf("Producer: Put %d into min_buffer at index %d\n", min, producer_min_put++);
	sem_post(&min_buffer_mutex);
	sem_post(&min_buffer_full);
}

int consumer_max_get = 0;
void *consumer_max(void *args) {
	int i;	
	for(i = 0; i < DIVIDE_NUM; i++) {
		sem_wait(&max_buffer_full);
		sem_wait(&max_buffer_mutex);
		if(max_buffer[consumer_max_get] > maximum) {
			maximum = max_buffer[consumer_max_get];
			printf("Consumer: Updated! maximum=%d\n", maximum);
		}
		consumer_max_get++;
		sem_post(&max_buffer_mutex);
	}
}

int consumer_min_get = 0;
void *consumer_min(void *args) {
	int i;	
	for(i = 0; i < DIVIDE_NUM; i++) {
		sem_wait(&min_buffer_full);
		sem_wait(&min_buffer_mutex);
		if(min_buffer[consumer_min_get] < minimum) {
			minimum = min_buffer[consumer_min_get];
			printf("Consumer: Updated! minimum=%d\n", minimum);
		}
		consumer_min_get++;
		sem_post(&min_buffer_mutex);
	}
}

int main() {
	srand(time(NULL));
	int i;
	for(i = 0; i < BIG_BUFFER_SIZE; i++) {
		big_buffer[i] = rand();
	}
	sem_init(&max_buffer_mutex, 0, 1);
	sem_init(&max_buffer_full, 0, 0);
	sem_init(&min_buffer_mutex, 0, 1);
	sem_init(&min_buffer_full, 0, 0);
	
	int order[DIVIDE_NUM];
	pthread_t p[DIVIDE_NUM], cmax, cmin;
	for(i = 0; i < DIVIDE_NUM; i++) {
		order[i] = i;
		pthread_create(&p[i], NULL, producer, &order[i]);
	}
	pthread_create(&cmax, NULL, consumer_max, NULL);
	pthread_create(&cmin, NULL, consumer_min, NULL);
	
	for(i = 0; i < DIVIDE_NUM; i++) {
		pthread_join(p[i], NULL);
	}
	pthread_join(cmax, NULL);
	pthread_join(cmin, NULL);
	

	sem_destroy(&max_buffer_mutex);
	sem_destroy(&max_buffer_full);
	sem_destroy(&min_buffer_mutex);
	sem_destroy(&min_buffer_full);
	
	printf("Success! maximum=%d and minimum=%d\n", maximum, minimum);
	
	return 0;
}
