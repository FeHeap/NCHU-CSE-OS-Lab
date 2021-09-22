#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
	FILE *cptr;
	int temp;
	printf("my real id=%d\n", getuid());
	printf("my effective id=%d\n", geteuid());
	cptr = fopen("datafile", "a");
	if(cptr == NULL) {
		printf("can't open file!\n");
		exit(1);
	}
	fprintf(cptr, "my real id=%d\n", getuid());
	fprintf(cptr, "my effective id=%d\n", geteuid());
	fclose(cptr);
	return 0;
}
