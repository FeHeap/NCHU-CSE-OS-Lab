#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#define TRUE 1

char pwdBuff[200], initialpwdBuff[200], lineHeadBuff[200];


void init();
void commandProcess(char*);
int commandStart = 0;

int main() {
	
	init();

	while(TRUE) {
		commandStart = 0;
		char *commandBuff;
		getcwd(pwdBuff, sizeof(pwdBuff));
		if(strlen(pwdBuff) >= strlen(initialpwdBuff)) {
			sprintf(lineHeadBuff, "\033[1;32m%s@%s-fesh\033[m:\033[1;34m%s~\033[m$", getenv("USER"), getenv("USER"), &pwdBuff[strlen(initialpwdBuff)]);
		}
		else {
			sprintf(lineHeadBuff, "\033[1;32m%s@%s-fesh\033[m:\033[1;34m%s\033[m$", getenv("USER"), getenv("USER"), pwdBuff);
		}
		
		commandBuff = readline(lineHeadBuff);
		if(strlen(commandBuff) != 0 && !isspace(commandBuff[0])) {
			add_history(commandBuff + commandStart);
		}
		rl_done = 0;
		commandProcess(commandBuff + commandStart);
		free(commandBuff);
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
		char *funcReturn = pointCmd->cmdFunc(command+i);
	}
	else {
		printf("%s: command not found\n", command);	
		return; 
	}
}

/* internal command functions */
char* cmd_cd(char *data) {
	int i = 0;
	while(isspace(data[i])) { i++; }
	if(data[i] == '\0') {
		chdir(initialpwdBuff);
		return NULL;	
	}

	data = data + i;
	int len = strlen(data);
	int j = 0;
	int argumentDetectState = 1;
	int spaceOK = 0;
	int endArgIndex = len;
	int k;
	for(k = 0; k < len; k++) {
		if(data[j] == '"') { spaceOK = !spaceOK; j++; }
		else if(argumentDetectState == 1 && (!isspace(data[j]) || spaceOK)) { j++; }
		else if(argumentDetectState == 1 && isspace(data[j])) { endArgIndex = j++; argumentDetectState = 2; }
		else if(argumentDetectState == 2 && isspace(data[j])) { j++; }
		else { argumentDetectState = 3; break; }
	}
	
	if(argumentDetectState == 3) {
		printf("fesh: cd: too many arguments\n");
		return NULL;
	}
	
	data[endArgIndex] = '\0';
	if(data[i] == '/') {
		if(chdir(data)) {
			printf("fesh: cd: %s: No such file or directory\n", data);
		}
	}
	else {
		getcwd(pwdBuff, sizeof(pwdBuff));
		strcat(pwdBuff, "/");
		strcat(pwdBuff, data);
		if(chdir(pwdBuff)) {
			printf("fesh: cd: %s: No such file or directory\n", data);
		}
	}

	return NULL;
}
char* cmd_echo(char *data) {
	int i = 0;
	while(isspace(data[i])) { i++; }
	data = data + i;

	int len = strlen(data);
	int inStringState = 0;
	for(i = 0; i < len; i++) {
		if(data[i] == '\\' && data[i+1] == '"') {
			i++;
		}
		else if(data[i] == '"') {
			inStringState = !inStringState;
			continue;		
		}
		printf("%c", data[i]);
		if(!inStringState && isspace(data[i])) {
			while(isspace(data[i])) { i++; }
			i--;
		}		
	}
	printf("\n");

	return NULL;
}
char* cmd_export(char *data) {
	printf("export inst\n");
	return NULL;
}
char* cmd_pwd(char *data) {
	getcwd(pwdBuff, sizeof(pwdBuff));
	printf("%s\n", pwdBuff);
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

	getcwd(pwdBuff, sizeof(pwdBuff));
	if(strlen(pwdBuff) >= strlen(initialpwdBuff)) {
		sprintf(lineHeadBuff, "\033[1;32m%s@%s-fesh\033[m:\033[1;34m%s~\033[m$", getenv("USER"), getenv("USER"), &pwdBuff[strlen(initialpwdBuff)]);
	}
	else {
		sprintf(lineHeadBuff, "\033[1;32m%s@%s-fesh\033[m:\033[1;34m%s\033[m$", getenv("USER"), getenv("USER"), pwdBuff);
	}
	printf("\n%s", lineHeadBuff);
	commandStart = rl_end;
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

