#include "MapDrawer.h"
#include <stdlib.h>

#define FONT_URL "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-BI.ttf"

void initMap();

int main()
{
	initMap();

	MapObjectType* fistObjectType = malloc(sizeof(MapObjectType));
	Color* blueColor = malloc(sizeof(Color));
	blueColor->red = 0x00;
	blueColor->green = 0x00;
	blueColor->blue = 0xff;
	fistObjectType->displayColor = blueColor;
	fistObjectType->name = "blueObject";
	fistObjectType->mapChar = 'b';
	fistObjectType->counter = 0;
	map_declareObjectType(fistObjectType);

	MapObjectType* secondObjectType = malloc(sizeof(MapObjectType));
	Color* greenColor = malloc(sizeof(Color));
	greenColor->red = 0x00;
	greenColor->green = 0xff;
	greenColor->blue = 0x00;
	secondObjectType->displayColor = greenColor;
	secondObjectType->name = "greenObject";
	secondObjectType->mapChar = 'g';
	secondObjectType->counter = 0;
	map_declareObjectType(secondObjectType);
	
	Point point;
	point.x = 0;
	point.y = 0;
	map_addObject(&point, 'b');
	point.x = 1;
	map_addObject(&point, 'a');
	point.x = 2;
	map_addObject(&point, 'g');

	map_displayAndWait();

	return 0;
}

void initMap()
{

	MapProperties* properties = malloc(sizeof(MapProperties));
	properties->title = "testMap";
	
	Dimensions* dimensions = malloc(sizeof(Dimensions));
	dimensions->width = 1000;
	dimensions->height = 800;
	properties->windowPixelSize = dimensions;

	properties->padding = 50;

	Color* blackColor = malloc(sizeof(Color));
	blackColor->red = 0;
	blackColor->green = 0;
	blackColor->blue = 0;
	properties->backgroundColor = blackColor;

	Color* whiteColor = malloc(sizeof(Color));
	whiteColor->red = 0xff;
	whiteColor->green = 0xff;
	whiteColor->blue = 0xff;
	properties->textColor = whiteColor;

	properties->agendaWidth = 200;

	Font* font = malloc(sizeof(Font));
	font->URL = FONT_URL;
	font->size = 20;
	properties->textFont = font;
	properties->textPadding = 5;

	Dimensions* mapSize = malloc(sizeof(Dimensions));
	mapSize->width = 3;
	mapSize->height = 3;
	properties->mapSize = mapSize;

	map_init(properties);
}