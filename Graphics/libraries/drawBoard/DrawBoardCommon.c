#include "DrawBoard.h"
#include "../tools/FileTool.h"
#include <stdlib.h>

#define COLOR_FORMAT "%d,%d,%d"

int readColorFromFileStreamByFormat(Color* color, char* format, FILE* fp);

void printPoint(Point* point)
{
	printf("{%d;%d}", point->x, point->y);
}

void copyPoint(Point** destinationPtr, Point* source)
{
	*destinationPtr = malloc(sizeof(Point));
	(*destinationPtr)->x = source->x;
	(*destinationPtr)->y = source->y;
}

void copyColor(Color** destinationPtr, Color* source)
{
	*destinationPtr = malloc(sizeof(Color));
	(*destinationPtr)->blue = source->blue;
	(*destinationPtr)->green = source->green;
	(*destinationPtr)->red = source->red;
}

int readColorFromFileStream(Color* color, FILE* fp)
{
	return readColorFromFileStreamByFormat(color, COLOR_FORMAT, fp);
}

int readColorPairFromFileStream(Color* color, char* key, FILE* fp)
{
	int res = readColorFromFileStreamByFormat(color, getKeyValueFormat(key, COLOR_FORMAT), fp);
	if (res)
		printf("Color key: %s\n", key);

	return res;
}

int readColorFromFileStreamByFormat(Color* color, char* format, FILE* fp)
{
	int res = fscanf(fp, format, &color->red, &color->green, &color->blue);
	int diff = 3 - res;
	if (diff)
		printf("Cannot read color\n");

	return diff;
}
