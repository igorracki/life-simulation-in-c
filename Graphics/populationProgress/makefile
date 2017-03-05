EXE=myapp

EXE: main.o ../libraries/lineChartDrawer/LineChartDrawer.o ../libraries/drawBoard/SDL_DrawBoard.o ../libraries/drawBoard/DrawBoardCommon.o ../libraries/tools/FileTool.o ../libraries/tools/List.o
	gcc -o myapp main.o ../libraries/lineChartDrawer/LineChartDrawer.o ../libraries/drawBoard/SDL_DrawBoard.o ../libraries/drawBoard/DrawBoardCommon.o ../libraries/tools/FileTool.o ../libraries/tools/List.o -lSDL2 -lSDL2_ttf -pthread

main.o: main.c ../libraries/lineChartDrawer/LineChartDrawer.h ../libraries/tools/FileTool.h
	gcc -c main.c
	
clean:
	rm -f *.o
	rm ${EXE}
