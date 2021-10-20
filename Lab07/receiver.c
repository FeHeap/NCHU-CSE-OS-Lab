#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#define name "/q2uque"
#define MAX_SIZE 1000
#define TRUE 1


int main(int argc, char **argv) {
	
	mqd_t mql;
	char buffer[MAX_SIZE+1];
	int must_stop = 0, tmp = 0;
	mql = mq_open(name, O_RDONLY|O_CREAT, 0644, NULL);
	
	if(mql == -1) {
		perror("mq error:\n");
		exit(EXIT_FAILURE);
	}
	
	do{
		ssize_t bytes_read;
		bytes_read = mq_receive(mql, buffer, 8192, NULL);
		if(bytes_read < 0) {
			perror("bytes_read error\n");
			exit(EXIT_FAILURE);
		}
		buffer[bytes_read] = '\0';
		printf("received: %s", buffer);
		if(strcmp(buffer, "exit\n") == 0) {
                	break;
		}
	}while(TRUE);
	
	int s_close = mq_close(mql);
	if(s_close == -1) {
		perror("s_close error:\n");
		exit(EXIT_FAILURE);
	}

	tmp = mq_unlink(name);

	if(tmp == -1) {
		printf("error");
		exit(EXIT_SUCCESS);
	}

	return 0;
}
