#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#define MSG_FILE "/etc/passwd"
#define MAX_SIZE 1000

struct msg_form {
	long mtype;
	char mtext[MAX_SIZE];
};

void* sender(int*);
void* receiver(int*);

int msqid;
pthread_t tsender;
pthread_t treceiver;

int main() {
	key_t key;
	struct msg_form msg;
	
	// get key
	if((key = ftok(MSG_FILE, 'z')) < 0) {
		perror("ftok error");	
		exit(EXIT_FAILURE);
	}

	if((msqid = msgget(key, IPC_CREAT|0777)) == -1) {
		perror("msgget error");
		exit(EXIT_FAILURE);
	}

	// thread setting
	void *ret;
	int here = 'A';
	int destination = 'B';
	if(pthread_create(&tsender, NULL, sender, &here)) {
		printf("CREATE ERROR\n");
		exit(EXIT_FAILURE);		
	}
	if(pthread_create(&treceiver, NULL, receiver, &destination)) {
		printf("CREATE ERROR\n");
		exit(EXIT_FAILURE);		
	}
	if(pthread_join(tsender, &ret)) {
		printf("JOIN ERROR\n");
		exit(EXIT_FAILURE);		
	}
	if(pthread_join(treceiver, &ret)) {
		printf("JOIN ERROR\n");
		exit(EXIT_FAILURE);		
	}

	return 0;	
}

void* sender(int *addr) {
	struct msg_form msg;
	msg.mtype = *addr;
	while(1) {
		printf("<Enter some text>\n");
		scanf("%s", msg.mtext);
		if(msgsnd(msqid, &msg, MAX_SIZE, 0) == -1) {
			printf("SEND ERROR\n");
			exit(EXIT_FAILURE);
		}
		if(!strcmp(msg.mtext, "exit")) {
			break;
		}
	}
	pthread_cancel(treceiver);
	pthread_exit(NULL);
}

void* receiver(int *addr) {
	struct msg_form msg;
	while(1) {
		msgrcv(msqid, &msg, MAX_SIZE, *addr, 0);
		printf("Received:%s\n", msg.mtext);
		if(!strcmp(msg.mtext, "exit")) {
			break;
		}
	}
	pthread_cancel(tsender);
	pthread_exit(NULL);
}
