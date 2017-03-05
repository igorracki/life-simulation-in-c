#ifndef DRAW_BOARD_H
#define DRAW_BOARD_H

#include <stdbool.h>
#include <stdio.h>

typedef struct
{
  short red;
  short green;
  short blue;
} Color;

typedef struct
{
  short x;
  short y;
} Point;

typedef struct
{
	short width;
	short height;
} Dimensions;

typedef struct
{
	unsigned short size;
	char* URL;
} Font;

typedef enum 
{
	LEFT, 
	RIGHT,
	MIDDLE
} HAlign;

typedef enum 
{
	TOP, 
	CENTER,
	BOTTOM
} VAlign;

int board_init(Dimensions* dimensions, char* title, Color* backgroundColor);
int board_displayDrawings();
int board_destroy();
void board_waitUntilCloseRequest();

int board_setDrawColor(Color* color);
int board_drawLine(Point* begin, Point* end);
int board_drawPoint(Point* point);
int board_drawRectangle(Point* beginPoint, Dimensions* dimensions, bool fill);
int board_drawText(char* text, Point* beginPoint, Font* font, HAlign hAlign, VAlign vAlign);
int board_drawX(Point* centerPoint, unsigned short size);

void printPoint(Point* point);
void copyPoint(Point** destinationPtr, Point* source);

void copyColor(Color** destinationPtr, Color* source);
int readColorPairFromFileStream(Color* color, char* key, FILE* fp);
int readColorFromFileStream(Color* color, FILE* fp);

#endif