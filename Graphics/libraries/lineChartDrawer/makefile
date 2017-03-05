EXE=myapp

EXE: test_lineChartDrawer.o LineChartDrawer.o ../drawBoard/SDL_DrawBoard.o ../drawBoard/DrawBoardCommon.o ../tools/FileTool.o ../tools/List.o
	gcc -o myapp test_lineChartDrawer.o LineChartDrawer.o ../drawBoard/DrawBoardCommon.o ../drawBoard/SDL_DrawBoard.o ../tools/FileTool.o ../tools/List.o -pthread -lSDL2 -lSDL2_ttf

test_lineChartDrawer.o: test_lineChartDrawer.c LineChartDrawer.h
	gcc -c test_lineChartDrawer.c -ggdb

LineChartDrawer.o: LineChartDrawer.h LineChartDrawer.c ../drawBoard/DrawBoard.h
	gcc -c LineChartDrawer.c
	
clean:
	rm -f *.o
	rm ${EXE}
