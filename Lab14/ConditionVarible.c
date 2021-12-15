#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static int glob = 0;

pthread_mutex_t mtx;
pthread_cond_t cond[3];


static void *threadFunc(void *arg) {
	int doWhat = *((int*)arg);
	int waitWhat = (doWhat == 0)? 2 : doWhat-1;
	int loc, s;

	while(1) {
		if(!(glob == 0 && doWhat == 0)) {
			s = pthread_cond_wait(&cond[waitWhat], &mtx);
		}

		////////////////////critical section////////////////////
		loc = doWhat*2 + 1;
		glob = loc;
		printf("%d\t", glob);
		////////////////////critical section////////////////////

		s = pthread_cond_signal(&cond[doWhat]);
	}
	return NULL;
}


int main(int argc, char *argv[]) {
	pthread_t t1, t2, t3;
	int s;
	s = pthread_mutex_init(&mtx, NULL);
	s = pthread_cond_init(&cond[0], NULL);
	s = pthread_cond_init(&cond[1], NULL);
	s = pthread_cond_init(&cond[2], NULL);
	
	int v1 = 0, v2 = 1, v3 = 2;
	s = pthread_create(&t1, NULL, threadFunc, &v1);
	if(s != 0) {
		printf("pthread_create ERROR\n");
	}
	s = pthread_create(&t2, NULL, threadFunc, &v2);
	if(s != 0) {
		printf("pthread_create ERROR\n");
	}
	s = pthread_create(&t3, NULL, threadFunc, &v3);
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
	s = pthread_cond_destroy(&cond[0]);
	s = pthread_cond_destroy(&cond[1]);
	s = pthread_cond_destroy(&cond[2]);
	return 0;
}
