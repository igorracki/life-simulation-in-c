all: main

main: main.c populateArray.o traverseArray.o moveFish.o printArray.o header.h getInput.o randomDirection.o
	gcc -Wall -g -o main main.c populateArray.o traverseArray.o moveFish.o printArray.o getInput.o randomDirection.o

populateArray.o: populateArray.c
	gcc -c populateArray.c
	
traverseArray.o: traverseArray.c
	gcc -c traverseArray.c

moveFish.o: moveFish.c
	gcc -c moveFish.c
	
printArray.o: printArray.c
	gcc -c printArray.c
	
getInput.o: getInput.c
	gcc -c getInput.c
	
randomDirection.o: randomDirection.c
	gcc -c randomDirection.c
	
clear:
	rm *.o -f
	