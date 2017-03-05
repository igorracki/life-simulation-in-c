#include "DrawBoard.h"

#define TITLE "Welcome!"
#define FONT_URL "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-BI.ttf"

int main()
{
	Dimensions dimensions;
	dimensions.width = 1000;
	dimensions.height = 800;
	
	Color color;
	color.red = 0;
	color.green = 0;
	color.blue = 0;
	
	board_init(&dimensions, TITLE, &color);
	
	color.red = 255;
	color.green = 255;
	color.blue = 255;
	board_setDrawColor(&color);
	
	Point point;
	point.x = 5;
	point.y = 5;
	board_drawPoint(&point);
	
	point.x = 10;
	point.y = 10;
	Point point2;
	point2.x = 200;
	point2.y = 200;
	board_drawLine(&point, &point2);
	
	dimensions.width = 100;
	dimensions.height = 100;
	board_drawRectangle(&point2, &dimensions, false);
	
	board_drawRectangle(&point, &dimensions, true);
	
	Font font;
	font.size = 15;
	font.URL = FONT_URL;
	board_drawText(FONT_URL, &point2, &font, RIGHT, TOP);
	
	color.blue = 0x00;
	color.green = 0x00;
	point.x = 200;
	point.y = 10;
	board_setDrawColor(&color);
	board_drawX(&point, 15);
	
	board_displayDrawings();
	board_waitUntilCloseRequest();
	board_destroy();
	return 0;
}