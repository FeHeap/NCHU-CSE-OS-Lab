#include <stdio.h>
#include "lib.h"

int main() {
	int arr[] = {15, 22, 41, 8, 35};
	int arr_size = sizeof(arr) / sizeof(int);
	printf("before sort is:");
	int i;
	for(i = 0; i < arr_size; i++) {
		printf(" %d", arr[i]);
	}
	sorting(arr, arr_size);
	printf("\nafter sort is:");
	for(i = 0; i < arr_size; i++) {
		printf(" %d", arr[i]);
	}
	printf("\n");
	
	return 0;
}
