#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv) {
	printf("Real Uid=%d\n", getuid());
	printf("Effective Uid=%d\n", geteuid());
	return 0;
}
