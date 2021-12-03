#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#define TRUE 1

char buff[200], initialpwdBuff[200], lineHeadBuff[200];

void init();
void commandProcess(char*);

int main() {
	
	init();

	char *ss;
	while(TRUE) {
		getcwd(buff, sizeof(buff));
		sprintf(lineHeadBuff, "\033[1;32m%s@%s-fesh\033[m:\033[1;34m%s~\033[m$", getenv("USER"), getenv("USER"), &buff[strlen(initialpwdBuff)]);
		ss = readline(lineHeadBuff);
		if(strlen(ss) != 0 && !isspace(ss[0])) {
			add_history(ss);
		}
		commandProcess(ss);
		free(ss);
	}
		
	return 0;
}

int stringHashCode(char *String) {
	int len = strlen(String);	
	int hashCode = 0;	
	int i;
	for(i = 0; i < len; i++) {
		hashCode += String[i] * 31 ^ (len - i);
	}
	return hashCode;
}



#define NumberOfCmd 4
typedef char* (*func_v_s)(char*);
struct cmdUnit {
	char *cmdName;	
	func_v_s cmdFunc;
	struct cmdUnit *next;
} cmdTable[NumberOfCmd];

void commandProcess(char *command) {
	int cmdLen = strlen(command);
	int i = 0;
	while(isspace(command[i])) { i++; }
	if(command[i] == '\0') { return; }
	else { command = command + i; }
	i = 0;	
	while(!isspace(command[i]) && command[i] != '\0') { i++; }
	if(command[i] == '\0') {  }
	else { command[i++] = '\0'; }
	int index = stringHashCode(command) % NumberOfCmd;
	if(cmdTable[index].cmdName == NULL) { 
		printf("%s: command not found\n", command);
		return; 
	}
	struct cmdUnit *pointCmd = cmdTable + index;
	while(pointCmd != NULL) {
		if(!strcmp(command, pointCmd->cmdName)) {
			break;
		}
		pointCmd = pointCmd->next;
	}
	if(pointCmd) {
		char *funcReturn = pointCmd->cmdFunc(NULL);
	}
	else {
		printf("%s: command not found\n", command);	
		return; 
	}
}

/* internal command functions */
char* cmd_cd(char *data) {
	printf("cd inst\n");
	return NULL;
}
char* cmd_echo(char *data) {
	printf("echo inst\n");
	return NULL;
}
char* cmd_export(char *data) {
	printf("export inst\n");
	return NULL;
}
char* cmd_pwd(char *data) {
	printf("pwd inst\n");
	return NULL;
}



/* initial function */
void cmdInit();
void cmdTerm();

void shell_close(int sig_num) {
	printf("interrupt!\n");
	cmdTerm();
	exit(EXIT_SUCCESS);
}

void shell_no_close(int sig_num) {
	getcwd(buff, sizeof(buff));
	sprintf(lineHeadBuff, "\033[1;32m%s@%s-fesh\033[m:\033[1;34m%s~\033[m$", getenv("USER"), getenv("USER"), &buff[strlen(initialpwdBuff)]);
	printf("\n%s", lineHeadBuff);
}



void init() {
	signal(SIGINT, shell_no_close);
	signal(SIGSEGV, shell_close);
	signal(SIGTERM, shell_close);
	signal(SIGHUP, shell_close);
	signal(SIGKILL, shell_close);
	signal(SIGQUIT, shell_close);
	signal(SIGBUS, shell_close);
	signal(SIGIOT, shell_close);
	signal(SIGTRAP, shell_close);
	
	cmdInit();	
	
	sprintf(initialpwdBuff, "/home/%s", getenv("USER"));	
	chdir(initialpwdBuff);	
}

void insertCmd(char *cmd, func_v_s func) {
	int index = stringHashCode(cmd) % NumberOfCmd;
	if(cmdTable[index].cmdName == NULL) {
		cmdTable[index].cmdName = cmd;
		cmdTable[index].cmdFunc = func;
	}
	else {
		struct cmdUnit *temp = (cmdTable + index);
		while(temp->next != NULL) { temp = temp->next; }
		temp->next = (struct cmdUnit*)malloc(sizeof(struct cmdUnit));
		temp = temp->next;
		temp->cmdName = cmd;
		temp->cmdFunc = func;
		temp->next = NULL;
	}
}

void cmdInit() {
	/* initialize cmdTable */
	int i;	
	for(i = 0; i < NumberOfCmd; i++) {
		cmdTable[i].cmdName = NULL;
		cmdTable[i].cmdFunc = NULL;
		cmdTable[i].next = NULL;
	}
	char *cmd = NULL;
	
	/* cd */
	cmd = (char*)malloc(3 * sizeof(char));
	strcpy(cmd, "cd");
	insertCmd(cmd, cmd_cd);
	cmd = NULL;

	/* echo */
	cmd = (char*)malloc(5 * sizeof(char));
	strcpy(cmd, "echo");
	insertCmd(cmd, cmd_echo);
	cmd = NULL;

	/* export */
	cmd = (char*)malloc(7 * sizeof(char));
	strcpy(cmd, "export");
	insertCmd(cmd, cmd_export);
	cmd = NULL;

	/* pwd */
	cmd = (char*)malloc(4 * sizeof(char));
	strcpy(cmd, "pwd");
	insertCmd(cmd, cmd_pwd);
	cmd = NULL;
}

void cmdTerm() {
	int i;	
	for(i = 0; i < NumberOfCmd; i++) {
		if(cmdTable[i].cmdName == NULL) {
			continue;		
		}
		free(cmdTable[i].cmdName);
		if(cmdTable[i].next != NULL) {
			struct cmdUnit *point = cmdTable[i].next;
			while(point != NULL) {
				struct cmdUnit *temp = point;
				point = point->next;
				free(temp->cmdName);
				free(temp);
			}
		}
	}
}

