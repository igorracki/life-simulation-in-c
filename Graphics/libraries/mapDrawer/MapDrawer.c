#include "MapDrawer.h"
#include "../tools/List.h"
#include <stdlib.h>

#define MIN_SECTOR_SIZE 1
#define AGENDA "Agenda"

#define CHAR_AREA sizeof(char) << 3

MapProperties* CURRENT_MAP_PROPERTIES;
Point BEGIN_POINT;
double X_SCALE, Y_SCALE;
MapObjectType* DECLARED_OBJECTS[CHAR_AREA];
List DECLARED_OBJECTS_LIST;
bool* MAPPED_SECTORS;
Dimensions SECTOR_PIXEL_SIZE;

sem_t DRAW_OBJECT;

void drawAgenda();

int map_init(MapProperties* properties)
{
	list_init(&DECLARED_OBJECTS_LIST);
	sem_init(&DRAW_OBJECT, 0, 1);

	CURRENT_MAP_PROPERTIES = properties;
	board_init(properties->windowPixelSize, properties->title, properties->backgroundColor);
	BEGIN_POINT.x = properties->padding;
	BEGIN_POINT.y = (properties->padding << 1) + properties->textFont->size;
	unsigned short mapWidth = properties->windowPixelSize->width - (properties->padding * 3) - properties->agendaWidth;
	unsigned short mapHeight = properties->windowPixelSize->height - BEGIN_POINT.y - properties->padding - properties->textFont->size - properties->textPadding;
	MAPPED_SECTORS = malloc(sizeof(bool) * properties->mapSize->width * properties->mapSize->height);
	X_SCALE = (double)mapWidth / properties->mapSize->width;
	Y_SCALE = (double)mapHeight / properties->mapSize->height;
	SECTOR_PIXEL_SIZE.width = (X_SCALE < MIN_SECTOR_SIZE) ? MIN_SECTOR_SIZE : X_SCALE;
	SECTOR_PIXEL_SIZE.height = (Y_SCALE < MIN_SECTOR_SIZE) ? MIN_SECTOR_SIZE : Y_SCALE;

	board_setDrawColor(properties->textColor);

	Point point;
	point.x = properties->windowPixelSize->width >> 1;
	point.y = properties->padding;
	board_drawText(properties->title, &point, properties->textFont, CENTER, BOTTOM);
	
	Dimensions mapFrameSize;
	mapFrameSize.width = mapWidth + 2;
	mapFrameSize.height = mapHeight + 2;
	point.x = BEGIN_POINT.x - 1;
	point.y = BEGIN_POINT.y - 1;
	board_drawRectangle(&point, &mapFrameSize, false);
	point.y += mapFrameSize.height + properties->textPadding;
	char* sizeLabel = malloc(sizeof(char) * 255);
	sprintf(sizeLabel, "Size: %d x %d", properties->mapSize->width, properties->mapSize->height);
	board_drawText(sizeLabel, &point, properties->textFont, RIGHT, BOTTOM);
	return 0;
}

int map_declareObjectType(MapObjectType* objectType)
{
	if (DECLARED_OBJECTS[objectType->mapChar])
		return 1;

	DECLARED_OBJECTS[objectType->mapChar] = objectType;
	list_addItem(&DECLARED_OBJECTS_LIST, (void*)objectType);
	return 0;
}

int map_addObject(Point* point, char mapChar)
{
	MapObjectType* objectType = DECLARED_OBJECTS[mapChar];
	if (objectType == NULL)
		return 1;

	objectType->counter++;

	Point sectorPoint;
	sectorPoint.x = (double)BEGIN_POINT.x + (double)point->x * X_SCALE;
	sectorPoint.y = (double)BEGIN_POINT.y + (double)point->y * Y_SCALE;
	//printPoint(&sectorPoint);
	//printf("\n");
	bool* isMapped = MAPPED_SECTORS + sectorPoint.y * CURRENT_MAP_PROPERTIES->mapSize->height + sectorPoint.x;
	if (*isMapped)
		return 1;

	*isMapped = true;

	sem_wait(&DRAW_OBJECT);
	board_setDrawColor(objectType->displayColor);
	board_drawRectangle(&sectorPoint, &SECTOR_PIXEL_SIZE, true);
	sem_post(&DRAW_OBJECT);

	return 0;
}

int map_displayAndWait()
{
	drawAgenda();
	board_displayDrawings();
	board_waitUntilCloseRequest();
}

void drawAgenda()
{
	board_setDrawColor(CURRENT_MAP_PROPERTIES->textColor);

	unsigned short textPadding = CURRENT_MAP_PROPERTIES->textPadding;
	unsigned short fontSize = CURRENT_MAP_PROPERTIES->textFont->size;
	unsigned short rowStartX = CURRENT_MAP_PROPERTIES->windowPixelSize->width - CURRENT_MAP_PROPERTIES->padding - CURRENT_MAP_PROPERTIES->agendaWidth;
	Point point;
	point.x = rowStartX;
	point.y = (CURRENT_MAP_PROPERTIES->windowPixelSize->height - (DECLARED_OBJECTS_LIST.length + 1) * (fontSize + textPadding)) >> 1;
	board_drawText(AGENDA, &point, CURRENT_MAP_PROPERTIES->textFont, RIGHT, BOTTOM);

	Dimensions dimensions;
	dimensions.width = fontSize;
	dimensions.height = fontSize;

	ListItem *listItem;
	for (listItem = DECLARED_OBJECTS_LIST.start; listItem; listItem = listItem->next)
	{
		MapObjectType* objectType = (MapObjectType*)listItem->item;

		point.x = rowStartX;
		point.y += fontSize + textPadding;
		board_setDrawColor(objectType->displayColor);
		board_drawRectangle(&point, &dimensions, true);

		point.x += fontSize + textPadding;
		char* objectLabel = malloc(sizeof(char) * 255);
		sprintf(objectLabel, "%s - %d", objectType->name, objectType->counter);
		board_setDrawColor(CURRENT_MAP_PROPERTIES->textColor);
		board_drawText(objectLabel, &point, CURRENT_MAP_PROPERTIES->textFont, RIGHT, BOTTOM);
	}

}