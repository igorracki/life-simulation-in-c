EXE=myapp

EXE: main.o ../libraries/mapDrawer/MapDrawer.o ../libraries/drawBoard/SDL_DrawBoard.o ../libraries/drawBoard/DrawBoardCommon.o ../libraries/tools/FileTool.o ../libraries/tools/List.o
	gcc -g -o myapp main.o ../libraries/mapDrawer/MapDrawer.o ../libraries/drawBoard/SDL_DrawBoard.o ../libraries/drawBoard/DrawBoardCommon.o ../libraries/tools/FileTool.o ../libraries/tools/List.o -lSDL2 -lSDL2_ttf -pthread

main.o: main.c ../libraries/mapDrawer/MapDrawer.h ../libraries/tools/FileTool.h
	gcc -c main.c
	
clean:
	rm -f *.o
	rm ${EXE}
