EXE=myapp

EXE: testDrawBoard.o SDL_DrawBoard.o DrawBoardCommon.o ../tools/FileTool.o ../tools/List.o
	gcc -o myapp testDrawBoard.o DrawBoardCommon.o SDL_DrawBoard.o ../tools/FileTool.o ../tools/List.o -lSDL2 -lSDL2_ttf -pthread

testDrawBoard.o: testDrawBoard.c DrawBoard.h
	gcc -c testDrawBoard.c

SDL_DrawBoard.o: DrawBoard.h SDL_DrawBoard.c
	gcc -c SDL_DrawBoard.c `pkg-config --cflags --libs sdl2`

DrawBoardCommon.o: DrawBoardCommon.c DrawBoard.h ../tools/FileTool.h
	gcc -c DrawBoardCommon.c
clean:
	rm -f *.o
	rm ${EXE}
