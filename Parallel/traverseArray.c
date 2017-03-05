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

void traverseArray() {
	int i = 0;
	int row = 0;
	int col = 0;
	double executionTime = 0;
	double start, finish, elapsed;
	int fishAlive = 0;
	int sharksAlive = 0;
	int *fishArr;
	int *sharkArr;
	long thread;
	pthread_t *thread_handles;
	
	if(controlFlag == 2){
		fishArr = malloc(year_no*sizeof(int));
		sharkArr = malloc(year_no*sizeof(int));		
	}
	
	while(i < year_no) {   
		//printf("\n---------- BEGINING OF THE YEAR %d ------------\n", i);
		//printArray();
		//traversing through the board and performing moveFish for each fish or shark 
		thread_handles = malloc(number_of_threads*sizeof(pthread_t));
		
		pthread_mutex_init(&my_mutex, NULL);
		
		GET_TIME(start);
		
		for(thread = 0; thread < number_of_threads; thread++) {
			pthread_create(&thread_handles[thread], NULL, modifyArray, (void *) thread);
		}
		
		for(thread = 0; thread < number_of_threads; thread++) {
			pthread_join(thread_handles[thread], NULL);
		}
		// When all cells of the board had performed move then cells is 
		// marked with minus sign, loops bellow remove the marker and 
		// makes the board ready for next cycle (next year)
		
		for(thread = 0; thread < number_of_threads; thread++) {
			pthread_create(&thread_handles[thread], NULL, cleanArray, (void *) thread);
		}
		
		for(thread = 0; thread < number_of_threads; thread++) {
			pthread_join(thread_handles[thread], NULL);
		}
		
		GET_TIME(finish);
		elapsed = finish - start;
		executionTime += elapsed;
		
		pthread_mutex_destroy(&my_mutex);
		// test check how many fishes and sharks is on the board no infuluence on Execution Time
		
		fishAlive = 0;
		sharksAlive = 0;
		for(row = 0; row < rows; row++){
			for(col = 0; col < cols; col++){
				if(arr[row*rows+col] % 2 == 1) {
					fishAlive++;
				}
				else if(arr[row*rows+col] != 0) {
					sharksAlive++;
				}
			}
		}
		
		if(controlFlag == 2){
			fishArr[i] = fishAlive;
			sharkArr[i] = sharksAlive;		
		}
         
		
// 		printf("---------- END OF THE YEAR %d \t Fishes alive: %d \t Sharks alive: %d \n", i, fishAlive, sharksAlive);
		
		i++; // while loop condition update
		
// 		if (sharksAlive ==0)
// 			break;
    }
    if (controlFlag == 1) {
		printf("At the end \t Fishes alive: %d \t Sharks alive: %d \n", fishAlive, sharksAlive);
		printf("Execution time is %e\n",executionTime);
	}
	if (controlFlag == 2) {
		printf("2\n%d\nSharks\n",year_no);
		for(i = 0; i < year_no; i++) {
			printf("%d ",sharkArr[i]);
		}
		printf("\nFishes\n");
		for(i = 0; i < year_no; i++) {
			printf("%d ", fishArr[i]);
		}
	}
	if (controlFlag == 3) {
		printf("2\nShark s\nFish f\n%d %d\n",rows, cols);
		printArray();
	}
}