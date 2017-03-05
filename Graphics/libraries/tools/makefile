EXE=myapp

EXE: test_fileTool.o FileTool.o List.o
	gcc -o myapp test_fileTool.o FileTool.o List.o -pthread

test_fileTool.o: test_fileTool.c FileTool.h
	gcc -c test_fileTool.c

FileTool.o: FileTool.h FileTool.c
	gcc -c FileTool.c
	
List.o: List.h List.c
	gcc -c List.c
	
clean:
	rm -f *.o
