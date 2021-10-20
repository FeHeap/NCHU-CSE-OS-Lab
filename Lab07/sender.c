#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <mqueue.h>
#define name "/q2uque"
#define MAX_SIZE 1000
#define TRUE 1


int main(int argc, char **argv) {
        mqd_t mql;
        char buffer[MAX_SIZE+1];
        mql = mq_open(name, O_WRONLY|O_CREAT, 0644, NULL);

        if(mql == -1) {
                perror("mq error:\n");
                exit(EXIT_FAILURE);
        }
	
	printf("send to receiver:\n");
        do{
		for(int i = 0; i < sizeof(buffer); i++) {
			buffer[i] = '\0';
		}
		printf(">");
		fgets(buffer, MAX_SIZE, stdin);
		int send = mq_send(mql, buffer, strlen(buffer)+1, 0);
		if(send < 0) {
			perror("message:\n");
			exit(EXIT_FAILURE);
		}
		if(strcmp(buffer, "exit\n") == 0) {
                        break;
                }
        }while(TRUE);

        return 0;
}

