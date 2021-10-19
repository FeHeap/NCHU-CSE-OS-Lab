#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {

	switch(fork()) {
		case -1:
			printf("fork error\n");
			exit(EXIT_FAILURE);
		case 0:	
			printf("I'm child process E\n");
			printf("Pid: %d\tParent pid: %d\n", getpid(), getppid());
			exit(EXIT_SUCCESS);

		default:wait(NULL);	
	}

	switch(fork()) {
		case -1:
			printf("fork error\n");
			exit(EXIT_FAILURE);
		case 0:	
			printf("I'm child process D\n");
			printf("Pid: %d\tParent pid: %d\n", getpid(), getppid());
			exit(EXIT_SUCCESS);

		default:wait(NULL);
	}

	switch(fork()) {
		case -1:
			printf("fork error\n");
			exit(EXIT_FAILURE);
		case 0:
			switch(fork()) {
				case -1:
					printf("fork error\n");
					exit(EXIT_FAILURE);
				case 0:
					printf("I'm child process C\n");
					printf("Pid: %d\tParent pid: %d\n", getpid(), getppid());
					exit(EXIT_SUCCESS);

				default:wait(NULL);			
			}

			printf("I'm child process B\n");
			printf("Pid: %d\tParent pid: %d\n", getpid(), getppid());
			exit(EXIT_SUCCESS);

		default:wait(NULL);			
	}

	printf("I'm parent process A\n");
	printf("Pid: %d\tParent pid: %d\n", getpid(), getppid());

	return 0;
}
