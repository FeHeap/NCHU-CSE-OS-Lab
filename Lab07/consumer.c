#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void) {
	const char *name = "OS";
	int shm_fd;
	char *shm_base, *ptr, buffer[1024];
	shm_fd = shm_open(name, O_RDONLY, 0666);
	if(shm_fd < 0) {
		perror("open error:\n");
		exit(EXIT_FAILURE);
	}
	shm_base = mmap(0, 1024, PROT_READ, MAP_SHARED, shm_fd, 0);
	if(shm_base == MAP_FAILED) {
		perror("shm_base error:\n");
		exit(EXIT_FAILURE);
	}
	printf("%s", shm_base);
	return 0;
}
