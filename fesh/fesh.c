#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#define PwdBufferSize 200
#define EvnBufferSize 2000
#define TRUE 1
#define FALSE 0

FILE *fout;
char feshHistoryPwd[PwdBufferSize];
char pwdBuff[PwdBufferSize], initialpwdBuff[PwdBufferSize], lineHeadBuff[PwdBufferSize];
int commandStart = 0;


void init();
void commandProcess(char*);

int main() {
	
	init();

	while(TRUE) {
		commandStart = 0;
		char *commandBuff;
		getcwd(pwdBuff, sizeof(pwdBuff));
		if(strlen(pwdBuff) >= strlen(initialpwdBuff)) {
			sprintf(lineHeadBuff, "\033[1;32m%s@%s-fesh\033[m:\033[1;34m~%s\033[m$ ", getenv("USER"), getenv("USER"), &pwdBuff[strlen(initialpwdBuff)]);
		}
		else {
			sprintf(lineHeadBuff, "\033[1;32m%s@%s-fesh\033[m:\033[1;34m%s\033[m$ ", getenv("USER"), getenv("USER"), pwdBuff);
		}
		
		commandBuff = readline(lineHeadBuff);
		if(strlen(commandBuff) != 0 && !isspace(commandBuff[0])) {
			add_history(commandBuff + commandStart);
		}
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
	return abs(hashCode);
}

int pwdDetermine(char *string) {
	int returnFlag = FALSE;
	int len = strlen(string);
	int i;
	for(i = 0; i < len; i++) {
		if(string[i] == '/') {
			returnFlag = TRUE;
			break;		
		}
	}
	return returnFlag;
}


void when_wait(int);
void when_fork(int);

#define NumberOfCmd 4
typedef char* (*func_s_iss)(int, char* []);
struct cmdUnit {
	char *cmdName;	
	func_s_iss cmdFunc;
	struct cmdUnit *next;
} cmdTable[NumberOfCmd];

void commandProcess(char *command) {
	int cmdLen = strlen(command);
	int i = 0;
	while(isspace(command[i])) { i++; }
	if(command[i] == '\0') { return; }
	else { command = command + i; }
	int inStringState = 0;
	i = 0;

	while(!isspace(command[i]) || inStringState == 1) {
		if(command[i] == '"') {
			inStringState = !inStringState;
		}
		else if(command[i] == '\\') {
			i++;
		}
		else if(command[i] == '\0') {
			break;				
		}
		i++;
	}

	int argFlag = FALSE;
	int bgFlag = FALSE;
	if(command[i-1] == '&') {
		bgFlag = TRUE;
		command[i-1] = '\0';
	}
	if(command[i] != '\0') {
		argFlag = TRUE;
		command[i++] = '\0';
	}

	struct cmdUnit *pointCmd = NULL;

	int index = stringHashCode(command) % NumberOfCmd;
	if(cmdTable[index].cmdName != NULL) {
		pointCmd = cmdTable + index;
		while(pointCmd != NULL) {
			if(!strcmp(command, pointCmd->cmdName)) {
				break;
			}
			pointCmd = pointCmd->next;
		}		
	}
		
		
	int argc = 0;
	char **argv = NULL;

	if(argFlag) {
		char *data = command + i;
		i = 0;
		inStringState = 0;
		while(TRUE) {
			while(isspace(data[i])) { i++; }
			if(data[i] == '\0') {
				break;		
			}

			argc++;
			while(!isspace(data[i]) || inStringState == 1) {
				if(data[i] == '"') {
					inStringState = !inStringState;
				}
				else if(data[i] == '\\') {
					i++;
				}
				else if(data[i] == '\0') {
					break;				
				}
				i++;
			}
			if(data[i] == '\0') {
				break;		
			}		
		}

		if(argc > 0) {
			argv = (char**)malloc(argc * sizeof(char*));
			i = 0;
			int j = 0;
			inStringState = 0;
			while(TRUE) {
				while(isspace(data[i])) { i++; }
				argv[j++] = data + i;
				while(!isspace(data[i]) || inStringState == 1) {
					if(data[i] == '"') {
						inStringState = !inStringState;
					}
					else if(data[i] == '\\') {
						i++;
					}
					else if(data[i] == '\0') {
						break;				
					}
					i++;
				}
				if(data[i] == '\0') {
					break;		
				}
				else {
					data[i++] = '\0';
				}		
			}

			if(argv[argc-1][0] == '&' && argv[argc-1][1] == '\0') {
				bgFlag = TRUE;
				argc--;
			}
			else if(argv[argc-1][strlen(argv[argc-1])-1] == '&') {
				bgFlag = TRUE;
				argv[argc-1][strlen(argv[argc-1])-1] = '\0';
			}
		}
	}

	if(pointCmd == NULL) {
		if(fork() == 0) {
			int exec_argc = argc;
			int change_stdout_flag = 0;
			if(argc >= 2) {
				if(!strcmp(argv[argc-2], ">>")) {
					fout = fopen(argv[argc-1], "a");
					change_stdout_flag = 1;
					exec_argc -= 2;
				}
				else if(!strcmp(argv[argc-2], ">")) {
					fout = fopen(argv[argc-1], "w");
					change_stdout_flag = 1;
					exec_argc -= 2;
				}
			}
			else if(argc >= 1) {
				if(argv[argc-1][0] == '>' && argv[argc-1][1] == '>') {
					if(argv[argc-1][2] != '\0') {
						argv[argc-1]+=2;
						fout = fopen(argv[argc-1], "a");
						change_stdout_flag = 1;
						exec_argc -= 1;
					}
					else {
						printf("fesh: syntax error near unexpected token `newline'\n");
						exit(EXIT_FAILURE);
					}
					
				}
				else if(argv[argc-1][0] == '>') {
					if(argv[argc-1][1] != '\0') {
						argv[argc-1]++;
						fout = fopen(argv[argc-1], "w");
						change_stdout_flag = 1;
						exec_argc -= 1;
					}
					else {
						printf("fesh: syntax error near unexpected token `newline'\n");
						exit(EXIT_FAILURE);
					}
				}
			}
			
			char** exec_argv = (char**)malloc((exec_argc+2) * sizeof(char*));

			exec_argv[0] = command;
			exec_argv[exec_argc+1] = NULL;
			for(i = 0; i < exec_argc; i++) {
				exec_argv[i+1] = argv[i];
			}
			when_fork(bgFlag);

			int save_fd;
			if(change_stdout_flag == TRUE) {
				printf("");
				save_fd = dup(STDOUT_FILENO);
				fout = fopen(argv[argc-1], "w");
				int fd = fileno(fout);
				dup2(fd, STDOUT_FILENO);
			}

			if(execvp(command, exec_argv) == -1) {
				if(change_stdout_flag == 1) {
					fclose(fout);				
					dup2(save_fd, STDOUT_FILENO);
				}
				if(pwdDetermine(command) == TRUE) {
					printf("fesh: %s: No such file\n", command);
				}
				else {
					printf("%s: command not found\n", command);
				}
				exit(EXIT_FAILURE);
			}
		}
		else {
			when_wait(bgFlag);
		}
		
	}
	else {

		char *funcReturn = pointCmd->cmdFunc(argc, argv);

		if(funcReturn != NULL) {
			if(argc >= 2) {
				if(!strcmp(argv[argc-2], ">>")) {
					fout = fopen(argv[argc-1], "a");
					fprintf(fout, "%s", funcReturn);
					fclose(fout);
				}
				else if(!strcmp(argv[argc-2], ">")) {					
					fout = fopen(argv[argc-1], "w");
					fprintf(fout, "%s", funcReturn);
					fclose(fout);
				}
				else {
					printf("%s\n", funcReturn);
				}
			}
			else if(argc >= 1) {
				if(argv[argc-1][0] == '>' && argv[argc-1][1] == '>') {
					if(argv[argc-1][2] != '\0') {
						fout = fopen(argv[argc-1]+2, "a");
						fprintf(fout, "%s", funcReturn);
						fclose(fout);
					}
					else {
						printf("fesh: syntax error near unexpected token `newline'\n");
					}
					
				}
				else if(argv[argc-1][0] == '>') {
					if(argv[argc-1][1] != '\0') {
						fout = fopen(argv[argc-1]+1, "w");
						fprintf(fout, "%s", funcReturn);
						fclose(fout);
					}
					else {
						printf("fesh: syntax error near unexpected token `newline'\n");
					}
				}
				else {
					printf("%s\n", funcReturn);
				}
			}
			else {
				printf("%s\n", funcReturn);
			}
		}
		
		free(funcReturn);
	}
	
	free(argv);	
}


/* internal command functions */
char* cmd_cd(int argc, char* argv[]) {
	if(argc > 1) {
		printf("fesh: cd: too many arguments\n");
		return NULL;
	}	

	if(argc == 0) {
		chdir(initialpwdBuff);
		return NULL;	
	}
	
	if(argv[0][0] == '/') {
		if(chdir(argv[0])) {
			printf("fesh: cd: %s: No such file or directory\n", argv[0]);
		}
	}
	else if(argv[0][0] == '~') {
		strcpy(pwdBuff, initialpwdBuff);
		strcat(pwdBuff, argv[0]+1);
		if(chdir(pwdBuff)) {
			printf("fesh: cd: %s: No such file or directory\n", argv[0]);
		}
	}
	else {
		getcwd(pwdBuff, sizeof(pwdBuff));
		strcat(pwdBuff, "/");
		strcat(pwdBuff, argv[0]);
		if(chdir(pwdBuff)) {
			printf("fesh: cd: %s: No such file or directory\n", argv[0]);
		}
	}

	return NULL;
}

char* cmd_echo(int argc, char* argv[]) {
	
	char words = 0;
	int i;
	for(i = 0; i < argc; i++) {
		if((i == argc-2 && (!strcmp(argv[i], ">") || !strcmp(argv[i], ">>"))) || (i == argc-1 && argv[i][0] == '>')) {
			break;
		}
		words += strlen(argv[i]);
	}
	
	char *echo = (char*)malloc(words * sizeof(char));
	int k = 0;
	for(i = 0; i < argc; i++) {
		if((i == argc-2 && (!strcmp(argv[i], ">") || !strcmp(argv[i], ">>"))) || (i == argc-1 && argv[i][0] == '>')) {
			break;
		}

		int j, len;
		if(argv[i][0] == '"') {
			j = 1;
			len = strlen(argv[i])-1;
		}
		else {
			j = 0;
			len = strlen(argv[i]);
		}
		for(; j < len; j++) {
			if(argv[i][j] == '\\') {
				j++;
			}
			echo[k++] = argv[i][j];
		}
		echo[k++] = ' ';
	}
	echo[k-1] = '\0';

	return echo;
}

char* cmd_export(int argc, char* argv[]) {

	char *export = NULL;

	if(argc == 2) {
		if(!strcmp(argv[1], ">>") || !strcmp(argv[1], ">")) {
			export = (char*)malloc(EvnBufferSize * sizeof(char));
			sprintf(export, "declare -x PATH=\"%s\"", getenv("PATH"));
		}
	}
	else if(argc == 1) {
		if(argv[0][0] == '>') {
			export = (char*)malloc(EvnBufferSize * sizeof(char));
			sprintf(export, "declare -x PATH=\"%s\"", getenv("PATH"));
		}
		else {
			char *envNameBuff = argv[0];
			int i;			
			for(i = 0; argv[0][i] != '=' && argv[0][i] != '\0'; i++);
			if(argv[0][i] == '=') {
				argv[0][i] = '\0';
				setenv(envNameBuff, argv[0]+i+1, 1);
			}
		}
	}
	else if(argc == 0) {
		export = (char*)malloc(EvnBufferSize * sizeof(char));
		sprintf(export, "declare -x PATH=\"%s\"", getenv("PATH"));
	}

	return export;
}

char* cmd_pwd(int argc, char* argv[]) {
	char *pwd = (char*)malloc(PwdBufferSize * sizeof(char));
	getcwd(pwd, PwdBufferSize * sizeof(char));
	return pwd;
}



/* initial function */
void cmdInit();
void cmdTerm();

void child_signal_handler_INT(int sig_num) {
	exit(EXIT_SUCCESS);
}

void shell_close(int sig_num) {
	printf("interrupt!\n");
	write_history(feshHistoryPwd);
	cmdTerm();
	exit(EXIT_SUCCESS);
}

void shell_no_close(int sig_num) {

	getcwd(pwdBuff, sizeof(pwdBuff));
	if(strlen(pwdBuff) >= strlen(initialpwdBuff)) {
		sprintf(lineHeadBuff, "\033[1;32m%s@%s-fesh\033[m:\033[1;34m~%s\033[m$ ", getenv("USER"), getenv("USER"), &pwdBuff[strlen(initialpwdBuff)]);
	}
	else {
		sprintf(lineHeadBuff, "\033[1;32m%s@%s-fesh\033[m:\033[1;34m%s\033[m$ ", getenv("USER"), getenv("USER"), pwdBuff);
	}
	printf("\n%s", lineHeadBuff);
	commandStart = rl_end;
}

/* TODO: catch ctrl + Z
void shell_bg(int sig_num) {
	
}
*/

void shell_wait_child(int sig_num) {
	int status;
	pid_t childPid = waitpid(-1, &status, WNOHANG);
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
	// signal(SIGTSTP, shell_bg);
	signal(SIGCHLD, shell_wait_child);
	
	cmdInit();

	getcwd(feshHistoryPwd, sizeof(feshHistoryPwd));
	strcat(feshHistoryPwd, "/.history");
	read_history(feshHistoryPwd);

	sprintf(initialpwdBuff, "/home/%s", getenv("USER"));	
	chdir(initialpwdBuff);
}

void insertCmd(char *cmd, func_s_iss func) {
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

/* for fork */
void when_fork(int bgFlag) {
	if(bgFlag == TRUE) {
		signal(SIGINT, SIG_IGN);
	}
	else {
		signal(SIGINT, child_signal_handler_INT);
	}
}

void when_wait(int bgFlag) {
	if(bgFlag == TRUE) {
		// pass
	}
	else {	
		signal(SIGINT, SIG_IGN);
		wait(NULL);
		signal(SIGINT, shell_no_close);
	}
}
