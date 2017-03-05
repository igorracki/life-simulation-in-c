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

void getInput(int argc, char* argv[]) {
	if(argc != 7) {
		rows = 10;
		cols = 10;
		number_of_fishes = 10;
		number_of_sharks = 3;
		year_no = 10;
		controlFlag = 3;
	}

	if(argc == 7){
		rows = atoi(argv[1]);
		cols = atoi(argv[2]);
		number_of_fishes = atoi(argv[3]);
		number_of_sharks = atoi(argv[4]);
		year_no = atoi(argv[5]);
		controlFlag = atoi(argv[6]);
		
	}
}