#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int globalVariable = 10;

int main() {
	int localVariable = 10;
	pid_t new_pid;

	new_pid = fork();
	
	switch(new_pid) {
		case -1:
			printf("fork error\n");
			break;
		case 0:
			printf("\n----------This is child process----------\n");
			globalVariable += 10;
			localVariable += 10;
			printf("Pid: %d\nParent pid: %d\n", getpid(), getppid());
			printf("globalVariable: %d, localVariable: %d\n", globalVariable, localVariable);			
			printf("-----------------------------------------\n");

			break;		
		default:
			wait(NULL);
			printf("\n----------This is parent process----------\n");
			printf("Pid: %d\nChild pid: %d\n", getpid(), new_pid);
			printf("globalVariable: %d, localVariable: %d\n", globalVariable, localVariable);
			printf("-----------------------------------------\n");			
			break;	
				
	}
	return 0;
}
