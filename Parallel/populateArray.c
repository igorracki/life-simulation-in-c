#include <stdio.h>
#include <stdlib.h>
#include "header.h"

extern int *arr;
extern int number_of_sharks;
extern int number_of_fishes;
extern int sea_size;
extern int year_no;
extern int rows;
extern int cols;


void populateArray() {
	// alocate space in memory for the board
	arr = malloc(rows*cols*sizeof(int));
	
	int i = 0;
	int positionX = 0;
	int positionY = 0;
	
	/** Initialize random number generator **/
	time_t t;
	srand((unsigned) time(&t));    

	/** IF array is empty	**/
	// place sharks on the bord
	while(i < number_of_sharks) {
		positionX = (rand() % rows);
		positionY = (rand() % cols);

		if(arr[positionX * rows + positionY] == 0){
			arr[positionX * rows + positionY] = SHARK_START_AGE;
			i++;	  
		}
    }
    i = 0;
    /************************/
    /** IF array is empty	**/
	//place fishes on the board 
    while(i < number_of_fishes) {
		positionX = (rand() % rows);
		positionY = (rand() % cols);
		
		if(arr[positionX*rows+positionY] == 0){
			arr[positionX*rows+positionY] = 1;
			i++;	  
		}
    }
  /************************/
  
}