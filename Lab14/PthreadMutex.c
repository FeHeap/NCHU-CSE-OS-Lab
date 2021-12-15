#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static int glob = 0;

pthread_mutex_t mtx;

static void *additionThreadFunc(void *arg) {
	int loops = *((int*)arg);
	int loc, i, s;

	for(i = 0; i < loops; i++) {
		s = pthread_mutex_lock(&mtx);
		if(s != 0) {
			printf("pthread_mutex_lock ERROR\n");
		}
		////////////////////critical section////////////////////
		loc = glob;
		loc+=2;
		glob = loc;
		////////////////////critical section////////////////////
		s = pthread_mutex_unlock(&mtx);
		if(s != 0) {
			printf("pthread_mutex_lock ERROR\n");
		}
	}
	return NULL;
}

static void *subtractionThreadFunc(void *arg) {
	int loops = *((int*)arg);
	int loc, i, s;

	for(i = 0; i < loops; i++) {
		s = pthread_mutex_lock(&mtx);
		if(s != 0) {
			printf("pthread_mutex_lock ERROR\n");
		}
		////////////////////critical section////////////////////
		loc = glob;
		loc-=2;
		glob = loc;
		////////////////////critical section////////////////////
		s = pthread_mutex_unlock(&mtx);
		if(s != 0) {
			printf("pthread_mutex_lock ERROR\n");
		}
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t t1, t2, t3;
	int loops, s;
	loops = atoi(argv[1]);
	s = pthread_mutex_init(&mtx, NULL);
	s = pthread_create(&t1, NULL, additionThreadFunc, &loops);
	if(s != 0) {
		printf("pthread_create ERROR\n");
	}
	s = pthread_create(&t2, NULL, additionThreadFunc, &loops);
	if(s != 0) {
		printf("pthread_create ERROR\n");
	}
	s = pthread_create(&t3, NULL, subtractionThreadFunc, &loops);
	if(s != 0) {
		printf("pthread_create ERROR\n");
	}

	s = pthread_join(t1, NULL);	
	if(s != 0) {
		printf("pthread_join ERROR\n");
	}
	s = pthread_join(t2, NULL);	
	if(s != 0) {
		printf("pthread_join ERROR\n");
	}
	s = pthread_join(t3, NULL);	
	if(s != 0) {
		printf("pthread_join ERROR\n");
	}
	printf("glob = %d\n", glob);
	s = pthread_mutex_destroy(&mtx);

	return 0;
}
