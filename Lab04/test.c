#define __GNU_SOURCE
#include <linux/unistd.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>
#define __NR_mysysyscall 436

int main(){
	syscall(__NR_mysysyscall); /*--or syscall(436)*/
	printf("Did I just finish?\nFind out in dmsg\n");

	return 0;
}
