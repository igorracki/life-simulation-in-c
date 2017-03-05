#include <stdio.h>
#include <stdlib.h>
#include "header.h"

//extern int number_of_sharks;
//extern int number_of_fishes;
//extern int sea_size;
//extern int year_no;
//extern int rows;
//extern int cols;

int randomDirection(int neighbors[]) {
	// function workrs only if neighbor array has a free cell == 0
	time_t t;
	srand((unsigned) time(&t));

	int flag = 1;
	int moveDirection;
	moveDirection = (int)(rand() % 4);
	
	while(flag == 1) {
		if(neighbors[moveDirection] == 0){
			flag = 0;
		} else {
			moveDirection = (moveDirection + 1) % 4;
		}
	}
	return moveDirection;
}