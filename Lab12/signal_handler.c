#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>



void catch_int_1(int sig_num) {
	printf("How are you?\n");
}

void catch_int_2(int sig_num) {
	printf("I am fine!\n");
}

void catch_alrm_2(int sig_num) {
	signal(SIGINT, SIG_DFL);
	signal(SIGALRM, SIG_DFL);
}

void catch_alrm_1(int sig_num) {
	signal(SIGINT, catch_int_2);
	signal(SIGALRM, catch_alrm_2);
	alarm(3);
}


int main() {
	signal(SIGINT, catch_int_1);
	signal(SIGALRM, catch_alrm_1);
	alarm(3);
	

	while(1);

	return 0;
}
