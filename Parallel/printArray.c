#include <stdio.h>
#include <stdlib.h>
#include "header.h"

extern int number_of_sharks;
extern int number_of_fishes;
extern int sea_size;
extern int year_no;
extern int rows;
extern int cols;
extern int controlFlag;

void printArray() { 
    int row = 0;
    int col = 0;

	for(row = 0; row < rows; row++){
		for(col = 0; col < cols; col++){
			if (arr[row*rows+col] == 0) {
				printf(" ");
			}
			else if(arr[row*rows+col] % 2 == 0) {
				printf("s");
			}
			else {
				printf("f");
			}
		}
	printf("\n");
	}
}