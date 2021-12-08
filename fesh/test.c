#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	printf("aa\n");
	int save_fd = dup(STDOUT_FILENO);
	// printf("%d\n", save_fd);
        FILE *fout = fopen("test.txt", "w");
	int fd = fileno(fout);
	// printf("%d\n", fd);
	// printf("%d\n", save_fd);
        dup2(fd, STDOUT_FILENO);
        printf("aaa");
	fclose(fout);
	printf("%d\n", STDOUT_FILENO);
	dup2(save_fd, STDOUT_FILENO);
	printf("%d\n", save_fd);
	printf("----------\n");
        printf("aaa\n");

        return 0;
}

