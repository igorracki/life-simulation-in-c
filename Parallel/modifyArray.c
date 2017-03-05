#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "header.h"
#include "timer.h"

extern int number_of_sharks;
extern int number_of_fishes;
extern int sea_size;
extern int year_no;
extern int rows;
extern int cols;
extern int controlFlag;
extern int number_of_threads;
extern pthread_mutex_t my_mutex;

void *modifyArray(void *rank) {
	long my_rank = (long) rank;
	int row, col;
	int local_rows = rows / number_of_threads;
	int my_first_row = my_rank * local_rows;
	int my_last_row = my_first_row + local_rows;
	
	for(row = my_first_row; row < my_last_row; row++){
		for(col = 0; col < cols; col++){
			if(arr[row*rows+col] > 0){
				pthread_mutex_lock(&my_mutex);
					moveFish(row, col); 
				pthread_mutex_unlock(&my_mutex);
			}
		}
	}
}
