#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int detectSleep = 0;

void *child(void *arg) {
	printf("child thread :\n");
	int *count = (int*)arg;
	count[2] = count[0] + count[1];
	detectSleep = 1;
	int i;
	for(i = 0; i < 10; i++) {
		printf("sleep : %d\n", detectSleep++);
		sleep(1);
	}
	pthread_exit((void*)&count[2]);
}

int main() {
	pthread_t t;
	int count[3] = {1, 2, 0};
	void *ret = &count[2];

	pthread_create(&t, NULL, child, (void*)count);

	while(detectSleep <= 3);
	if(pthread_cancel(t) == 0) {
		printf("pthread_cancel successfully\n");
	}
	else {
		printf("pthread_cancel failed\n");
		pthread_join(t, &ret);
	}

	printf("main thread :\n");
	printf("%d + %d = %d\n", count[0], count[1], *((int*)ret));

	return 0;
}
