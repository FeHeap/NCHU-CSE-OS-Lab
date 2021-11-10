#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int count = 0;

void* countFunc(void *arg) {
	int i;
	for(i = 0; i < 5; i++) {
		printf("Thread ID is: %lu, num is %d\n", pthread_self(), ++count);
	}
	printf("Thread, ID is: %lu Completed\n", pthread_self());
	pthread_exit(NULL);
}



int main() {
	pthread_t thread[4];
	int rc;
	void *ret;
	int i;
	for(i = 0; i < 4; i++) {
		rc = pthread_create(&thread[i], NULL, countFunc, NULL);
		if(rc) {
			printf("CREATE ERROR\n");
			exit(EXIT_FAILURE);		
		}
	}

	for(i = 0; i < 4; i++) {
		rc = pthread_join(thread[i], &ret);
		if(rc) {
			printf("JOIN ERROR\n");
			exit(EXIT_FAILURE);		
		}
	}
	printf("done! count is %d\n", count);

	return 0;
}
