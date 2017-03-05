EXE=myapp

EXE: test_mapDrawer.o MapDrawer.o ../drawBoard/SDL_DrawBoard.o ../drawBoard/DrawBoardCommon.o ../tools/List.o ../tools/FileTool.o
	gcc -o myapp test_mapDrawer.o MapDrawer.o ../drawBoard/SDL_DrawBoard.o ../drawBoard/DrawBoardCommon.o ../tools/List.o ../tools/FileTool.o -lSDL2 -lSDL2_ttf -pthread

test_mapDrawer.o: test_mapDrawer.c MapDrawer.h
	gcc -c test_mapDrawer.c

MapDrawer.o: MapDrawer.h MapDrawer.c ../drawBoard/DrawBoard.h ../tools/List.h
	gcc -c MapDrawer.c
	
clean:
	rm -f *.o
	rm ${EXE}
