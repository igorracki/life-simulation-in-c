#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(int argc, char* argv[]) {
	// procede input parameters 
    getInput(argc, argv);
	
	// create and populate booard with fishes and sharks
    populateArray();
	
	// test printing of board
  //  printArray();
	traverseArray();
    //printArray();
    
    return 0;
}