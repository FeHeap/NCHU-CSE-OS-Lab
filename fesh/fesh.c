#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#define TRUE 1

char buff[200], initialpwdBuff[200];

int main() {
	sprintf(initialpwdBuff, "/home/%s", getenv("USER"));	
	chdir(initialpwdBuff);
	// printf("current working directory: %s\n", getcwd(buff, sizeof(buff)));
	
	char *ss, *ps;
	while(TRUE) {
		getcwd(buff, sizeof(buff));
		char lineHeadBuff[200];
		sprintf(lineHeadBuff, "\033[1;32m%s@%s-fesh\033[m:\033[1;34m%s~\033[m$", getenv("USER"), getenv("USER"), &buff[strlen(initialpwdBuff)]);
		printf("%s", line);
		ss = readline(ps);
	}
		
	return 0;
}
