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

// moveFish function used to meve fish or shark on the board
int moveFish(int row, int col) {
	
	int neighbors[4]; // index 0 = Top, 1 = Right, 2 = Bottom, 3 = Left
	
	int *top;
	int *right;
	int *bottom;
	int *left;
	
	int i = 0;
	int sharkAte = 0;
	
	// check if is fish or shark if fish 
	// variable isFish = 1 if is shark isFish = 2
	int isFish = arr[(row*rows)+col] % 2;
	
	// calculate age of the fish/shark
	int fishAge = arr[(row*rows)+col] /2;
	
	/***************** Find neighbours in four directions *******************************/
	//Finding top neighbour
	if(row == 0) {
		neighbors[0] = arr[(rows-1)*rows+col];
		top = &arr[(rows-1)*rows+col];
	} else {
		neighbors[0] = arr[(row-1)*rows+col];
		top = &arr[(row-1)*rows+col];
	}
    
    //Finding right neighbour
    if(col == cols-1){
		neighbors[1] = arr[row*rows];
		right = &arr[row*rows];
	} else {
		neighbors[1] = arr[row*rows+(col+1)];
		right = &arr[row*rows+(col+1)];
	}
    
    //Finding bottom neighbour
    if(row == rows-1){
		neighbors[2] = arr[col];
		bottom = &arr[col];
    } else {
		neighbors[2] = arr[(row+1)*rows+col];
		bottom = &arr[(row+1)*rows+col];
    }
    
    //Finding left neighbour
    if(col == 0){
		neighbors[3] = arr[row*rows+(cols-1)];
		left = &arr[row*rows+(cols-1)];
    } else {
		neighbors[3] = arr[row*rows+(col-1)];
		left = &arr[row*rows+(col-1)];
    }
    
    /*********** FINDING OUT THE NUMBERS NEIGHBOURS  ************************/
	// numbers of neighbours fishes, sharks and empty cells is needed to decide what action animal perform 
    int emptyCells = 0;
    int neighborFishes = 0;
    int neighborSharks = 0;
//    printf("Neighbour array ");
    for(i = 0; i < 4; i++){
// 		printf("%d, ",neighbors[i]);
		if(neighbors[i] == 0){ 				//empty cell
			emptyCells++;
		} else if(neighbors[i] % 2 == 1){ 	// fish in the cell
			if(neighbors[i] > 0){ 			// check if the cell already updated in current cycle (if number < 0 cell is used before)
				neighborFishes++;
			}
		} else if(neighbors[i] % 2 == 0){	//shark in the cell
			if(neighbors[i] > 0){
				neighborSharks++;
			}
		}
    }  
//    printf(", emptyCells: %d, neighborFishes: %d, neighborSharks: %d\n", emptyCells, neighborFishes, neighborSharks);
	/**************************	ACTIONS FOR FISH ONLY	********************************************************/
	if(isFish == 1) {
// 		printf("Fish located at %d, %d ", row+1, col+1);
		
		/************** KILL FISH FROM OVER-CROWDING	********************/
		// Fish has 3 neighbers fishes 
		if(neighborFishes > 2){
// 			printf("has DIED OVER-CROWDING\n");
			arr[row*rows+col] = 0;
// 			return 0;
		} 
		
		// Fish stays in the same place only when is surrendered by sharks or fishes from previous iteration 
		else if (emptyCells == 0) {
			arr[row*rows+col] = (arr[row*rows+col] + 2) * (-1);
// 			printf("has stayed in same location\n");
// 			return 0;
		}
		
		/*************** MOVE FISH TO A RANDOM NEXT CELL	********************/
		else if (neighborFishes < 2 && emptyCells > 0){
			int fishMoveDir = randomDirection(neighbors);

			if(fishMoveDir == 0) {
				*top = (arr[row*rows+col] + 2) * (-1);
				arr[row*rows+col] = 0;
			} else if(fishMoveDir == 1){
				*right = (arr[row*rows+col] + 2) * (-1);
				arr[row*rows+col] = 0;
			} else if(fishMoveDir == 2){
				*bottom = (arr[row*rows+col] + 2) * (-1);
				arr[row*rows+col] = 0;
			} else {
				*left = (arr[row*rows+col] + 2) * (-1);
				arr[row*rows+col] = 0;
			}
// 			printf("has moved %d\n", fishMoveDir);
//  			return 0;
		  
		
		/************* REPRODUCE FISH IF THERE ARE 2 FISH AROUND	************/
		} else if (neighborFishes == 2 && emptyCells > 0) {

			int spawnDirection = randomDirection(neighbors);
			// Fish is born in the new spawn position with value -1 to not participated any more in this cycle 
			// Fish who gave birth is aged and marked -1
			if(spawnDirection == 0) {
				*top = -1;
				arr[row*rows+col] = (arr[row*rows+col] + 2) * (-1);
			} else if(spawnDirection == 1){
				*right = -1;
				arr[row*rows+col] = (arr[row*rows+col] + 2) * (-1);
			} else if(spawnDirection == 2){
				*bottom = -1;
				arr[row*rows+col] = (arr[row*rows+col] + 2) * (-1);
			} else {
				*left = -1;
				arr[row*rows+col] = (arr[row*rows+col] + 2) * (-1);
			}
// 			printf("has gave birth to %d\n", spawnDirection);
// 			return 0;
		}
    
	/******************* ACTIONS FOR SHARK ONLY	****************************************************************/
	} else {
// 		printf("SHARK located at %d, %d ", row+1, col+1);
		int randomDir;
		
		/************** KILL SHARK FROM STARVATION or died of old age	**************************/
		if (fishAge > SHARK_LIFE || neighborSharks > 3){ 
			arr[(row*rows)+col] = 0;
// 			printf("has DIED of old age\n");
		}
		
		/*************** REPRODUCE SHARK, LOOK FOR FOOD AND MOVE	************/
		else if (fishAge > SHARK_BIRTH_TIME && neighborSharks > 0 && emptyCells > 0) {
			
			// age of the shark is good to reproduce and it has other shark as a neighbour
			// now we need to check:
			// if there is any shark in neighbourhood is old enough to have a baby
			
			// traversal neighbours array checking age of shark and if is old enough
			// respawning baby shark in random direction
			randomDir = randomDirection(neighbors);
			for (i = 0; i < 4; i++) {
				/*********** REPRODUCING	*********/
				if (neighbors[i] > SHARK_BIRTH_TIME){
					if (randomDir == 0) {
						*top = -20;
						arr[row*rows+col] = (arr[row*rows+col] + 2) * (-1);
					} else if(randomDir == 1){
						*right = -20;
						arr[row*rows+col] = (arr[row*rows+col] + 2) * (-1);
					} else if(randomDir == 2){
						*bottom = -20;
						arr[row*rows+col] = (arr[row*rows+col] + 2) * (-1);
					} else {
						*left = -20;
						arr[row*rows+col] = (arr[row*rows+col] + 2) * (-1);
					}
// 					printf("has give a birth to %d\n", randomDir);
					break;
				}
			}
		}
		
		// now shark can only eat or move
		// it can eat if there is a fish in neighbourhood
		else if (neighborFishes > 0) {
			// traversal neighbour array looking for first presents of fish
			// when sharks find a fish it moves to fish position eating it
			for(i = 0; i < 4; i++){
				/************ LOOKING FOR FOOD *************/
				if(neighbors[i] % 2 == 1 && neighbors[i] > 0 ){
					if (i == 0) {
						*top = (arr[row*rows+ col] + 2) * (-1);
						arr[row*rows+col] = 0;
					} else if (i == 1){
						*right = (arr[row*rows+col]+2) * (-1);
						arr[row*rows+col] = 0;
					} else if (i == 2){
						*bottom = (arr[row*rows+col]+2) * (-1);
						arr[row*rows+col] = 0;
					} else {
						*left = (arr[row*rows+col]+2) * (-1);
						arr[row*rows+col] = 0;
					}
					
// 					printf("has ate a fish from the %d\n", i);
					break;
				}
			}
		}

		/************ MOVING SHARK IF IT CAN'T REPRODUCE	********************/
		// there is no sharks ready to reproduce or fish to eat so shark can only move
		// but it need free cell 
		else if (neighborFishes == 0 && emptyCells > 0) {
			randomDir = randomDirection(neighbors);
			// move a shark to random free neighbour cell (it can be cell occupied by a young shark) 
			if (randomDir == 0) {
				*top = (arr[row*rows+col] - 2) * (-1);
				arr[row*rows+col] = 0;
			} else if (randomDir == 1){
				*right = (arr[row*rows+col] - 2) * (-1);
				arr[row*rows+col] = 0;
			} else if (randomDir == 2){
				*bottom = (arr[row*rows+col] - 2) * (-1);
				arr[row*rows+col] = 0;
			} else {
				*left = (arr[row*rows+col] - 2) * (-1);
				arr[row*rows+col] = 0;
			}
			// testing 
// 			printf("has move to the %d\n", randomDir);
		}
		
		// shark has no empty cell (all four neighbour animals are marked as moved(-1))
		else {
			arr[row*rows+col] = (arr[row*rows+col] - 2) * (-1);
		
			// testing 
// 			if (arr[row*rows+col] == 0) {
// 				printf("has DIED of STARVATION\n");
// 			} else {
// 				printf("has stayed in same location\n");
// 			}
			
		}

	} // end of shark action
} //end of function moveFish
