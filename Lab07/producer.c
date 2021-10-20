#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void) {
	const char *name = "OS";
	int shm_fd;
	char *shm_base, *ptr, buffer[1024];
	shm_fd = shm_open(name, O_CREAT|O_RDWR, 0666);
	if(shm_fd < 0) {
		perror("open error:\n");
		exit(EXIT_FAILURE);
	}
	ftruncate(shm_fd, 1024);
	shm_base = mmap(0, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(shm_base == MAP_FAILED) {
		perror("shm_base error:\n");
		exit(EXIT_FAILURE);
	}
	
	
	fgets(buffer, 1023, stdin);
	ptr = shm_base;
	ptr += sprintf(ptr, "%s", buffer);
	if(munmap(shm_base, 1024) == -1) {
		perror("munmap error:\n");
		exit(EXIT_FAILURE);
	}
	if(close(shm_fd) == -1) {
		perror("close error:\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
