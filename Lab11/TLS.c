#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static __thread int glob = 0;

void *incr(void *args) {
	int threadNumber = *((int*)args+0);
	int increaseTime = *((int*)args+1);
	int i;
	for(i = 0; i < increaseTime; i++) {
		glob++;
	}
	printf("thread%d , glob = %d\n", threadNumber, glob);
	pthread_exit(NULL);
}

int main() {
	printf("(main thread)glob value before running 2 : %d\n", glob);
	
	pthread_t id[2];
	int threads_args[2][2] = {{1, 50}, {2, 100}};
	pthread_create(&id[0], NULL, incr, threads_args[0]);
	pthread_create(&id[1], NULL, incr, threads_args[1]);
	pthread_join(id[0], NULL);
	pthread_join(id[1], NULL);

	printf("(main thread)glob value after running 2 : %d\n", glob);
	
	return 0;
}
