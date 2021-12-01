#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
	switch(fork()) {
		case -1:
			printf("fork error!\n");
			exit(EXIT_FAILURE);
		case 0:
			sleep(1);
			printf("I am child, pid=%hd and ppid=%hd\n", getpid(), getppid());
			kill(getpid(), SIGTERM);
			exit(EXIT_SUCCESS);
		default:
			printf("I am parent, pid=%hd and ppid=%hd\n", getpid(), getppid());
			wait(NULL);
			kill(getpid(), SIGTERM);	
	}

	return 0;
}
