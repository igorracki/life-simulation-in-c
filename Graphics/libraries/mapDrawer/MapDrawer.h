#ifndef MAP_DRAWER_H

#define MAP_DRAWER_H

#include "../drawBoard/DrawBoard.h"
#include <semaphore.h> 

typedef struct {
	Dimensions* windowPixelSize;
	Dimensions* mapSize;
	char* title;
	Font *textFont;
	unsigned short agendaWidth;
	unsigned short padding;
	unsigned short textPadding;
	Color* backgroundColor;
	Color* textColor;
}MapProperties;

typedef struct {
	char mapChar;
	char* name;
	Color* displayColor;
	unsigned short counter;
}MapObjectType;

int map_init(MapProperties* properties);
int map_declareObjectType(MapObjectType* objectType);
int map_addObject(Point* point, char mapChar);
int map_displayAndWait();

#endif // !MAP_DRAWER_H

