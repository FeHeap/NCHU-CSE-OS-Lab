#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	srand(time(NULL));
	char physical_memory[16][2] = {};
	int free_frame_table[16] = {};
	char A_logical_memory[3][2] = {{'a', 'b'}, {'e', 'f'}, {'c', 'd'}};
	int A_page_table[3] = {};
	char B_logical_memory[3][2] = {{'r', 'x'}, {'y', 'y'}, {'z', 'z'}};
	int B_page_table[3] = {};
	
	int i, j, full;
	for(i = 0; i < 3; i++) {
		int index = rand() % 16;
		for(j = index, full = 0; full < 16; j=(j+1)%16, full++) {
			if(free_frame_table[j] == 0) {
				physical_memory[j][0] = A_logical_memory[i][0];
				physical_memory[j][1] = A_logical_memory[i][1];
				A_page_table[i] = j;
				free_frame_table[j] = 1;
				break;
			}
		}
	}
	for(i = 0; i < 3; i++) {
		int index = rand() % 16;
		for(j = index, full = 0; full < 16; j=(j+1)%16, full++) {
			if(free_frame_table[j] == 0) {
				physical_memory[j][0] = B_logical_memory[i][0];
				physical_memory[j][1] = B_logical_memory[i][1];
				B_page_table[i] = j;
				free_frame_table[j] = 1;
				break;
			}
		}
	}
	
	
	printf("process1's Logical memory:\n");
	printf("page	data\n");
	for(i = 0; i < 3; i++) {
		printf("%d	%c%c\n", i, A_logical_memory[i][0], A_logical_memory[i][1]);
	}
	printf("process1's page table:\n");
	printf("page	frame\n");
	for(i = 0; i < 3; i++) {
		printf("%d	%d\n", i, A_page_table[i]);
	}
	printf("process2's Logical memory:\n");
	printf("page	data\n");
	for(i = 0; i < 3; i++) {
		printf("%d	%c%c\n", i, B_logical_memory[i][0], B_logical_memory[i][1]);
	}
	printf("process2's page table:\n");
	printf("page	frame\n");
	for(i = 0; i < 3; i++) {
		printf("%d	%d\n", i, B_page_table[i]);
	}
	
	printf("physical memory is:\n");
	printf("frame	offset	data\n");
	for(i = 0; i < 16; i++) {
		printf("%d	0	%c\n", i, physical_memory[i][0]);
		printf("%d	1	%c\n", i, physical_memory[i][1]);
	}
	printf("free-frame table is:\n");
	for(i = 0; i < 16; i++) {
		if(free_frame_table[i] == 0) {
			printf("%d ", i);
		}
	}
	printf("\n");
	
	return 0;
}
