#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
        int fd = open("test.txt", O_WRONLY|O_CREAT);
        dup2(fd, STDOUT_FILENO);
        printf("aaa");

        return 0;
}

