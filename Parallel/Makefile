all: main

main: main.c populateArray.o traverseArray.o moveFish.o printArray.o header.h timer.h getInput.o randomDirection.o modifyArray.o cleanArray.o
	gcc -Wall -g -o main main.c populateArray.o traverseArray.o moveFish.o printArray.o getInput.o randomDirection.o modifyArray.o cleanArray.o -lpthread

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
	
modifyArray.o: modifyArray.c
	gcc -c modifyArray.c

cleanArray.o: cleanArray.c
	gcc -c cleanArray.c
	
clear:
	rm *.o -f
	