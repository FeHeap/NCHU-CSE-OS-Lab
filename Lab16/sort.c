#include "lib.h"

void sorting(int *arr, int size) {
	int i, j, imax, temp;
	int isize = size-1;
	for(i = 0; i < isize; i++) {
		imax = i;
		for(j = i+1; j < size; j++) {
			if(arr[j] > arr[imax]) {
				imax = j;
			}
		}
		temp = arr[i];
		arr[i] = arr[imax];
		arr[imax] = temp;
	}
}
