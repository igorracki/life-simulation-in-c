#include "LineChartDrawer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LINES 10
#define AGENDA_STR "Agenda"

typedef enum {HORIZONTAL, UP} Orientation;

Dimensions chartSize;
Point chartStart;
LineChartProperties* currentProperties;
double scaleX, scaleY;
short currentDataLine = -1;
LineData* dataLines[MAX_LINES + 1];

void initChartParameters();
void drawAxis(Axis* axis, Orientation orientation);
void drawTitle();
void drawShape(Point* centerPoint, Shape shape, unsigned short size);
void drawAgenda();
void getPoint(Point* point, double xData, double yData);
unsigned short getX(double xData);
unsigned short getY(double yData);

int chart_initChart(LineChartProperties* properties)
{
	currentProperties = properties;	
	board_init(currentProperties->size, currentProperties->name, currentProperties->backgroundColor);
	board_setDrawColor(currentProperties->color);
	initChartParameters();
	drawTitle();
	drawAxis(currentProperties->xAxis, HORIZONTAL);
	drawAxis(currentProperties->yAxis, UP);
	
}

int chart_defineLineData(LineData * lineData)
{

	if (currentDataLine < MAX_LINES)
	{	
		currentDataLine ++;
		dataLines[currentDataLine] = lineData;
	}

	board_setDrawColor(lineData->color);
	return 0;
}

int chart_putData(double xData, double yData)
{
	//printf("putData %f %f %d\n", xData, yData, currentDataLine);
	Point point;
	getPoint(&point, xData, yData);
	//printf("dataLine: %d %d %d %d\n", dataLines, dataLines[currentDataLine], dataLines[currentDataLine]->shape, dataLines[currentDataLine]->size);
	drawShape(&point, dataLines[currentDataLine]->shape, dataLines[currentDataLine]->size);
	return 0;
}

int chart_showChart()
{
	drawAgenda();

	board_displayDrawings();
	return 0;
}

int chart_waitAndClose()
{
	board_waitUntilCloseRequest();
	board_destroy();
	return 0;
}


void initChartParameters()
{
	chartSize.width = currentProperties->size->width - (currentProperties->padding * 3) - currentProperties->agendaWidth;
	chartSize.height = currentProperties->size->height - (currentProperties->padding << 1);
	chartStart.x = currentProperties->padding;
	chartStart.y = currentProperties->size->height - currentProperties->padding;
	
	scaleX = (currentProperties->xAxis->maxData - currentProperties->xAxis->minData) / chartSize.width;
	scaleY = (currentProperties->yAxis->maxData - currentProperties->yAxis->minData) / chartSize.height;
	
	printf("Chart parameters:\nSize: %d x %d\nStart: ", chartSize.width, chartSize.height);
	printPoint(&chartStart);
	printf("\nScales: x: %f, y: %f\n", scaleX, scaleY);
}

void drawAxis(Axis* axis, Orientation orientation)
{
	printf("Drawing axis %s\n", axis->name);
	Point startPoint;
	Point endPoint;
	Point textPoint;
	HAlign hAlign;
	VAlign vAlign;
	switch(orientation)
	{
		case HORIZONTAL:
			getPoint(&startPoint, axis->minData, axis->position);
			endPoint.x = startPoint.x + chartSize.width;
			endPoint.y = startPoint.y;
			textPoint.x = endPoint.x + currentProperties->textPadding;
			textPoint.y = endPoint.y;
			hAlign = RIGHT;
			vAlign = CENTER;
			break;
		case UP:
			getPoint(&startPoint, axis->position, axis->minData);
			endPoint.x = startPoint.x;
			endPoint.y = startPoint.y - chartSize.height;
			textPoint.x = endPoint.x;
			textPoint.y = endPoint.y - currentProperties->textPadding;
			hAlign = MIDDLE;
			vAlign = TOP;
			break;
	}
	
	printf("Points: ");
	printPoint(&startPoint);
	printf(" ");
	printPoint(&endPoint);
	printf("\n");
	board_drawLine(&startPoint, &endPoint);
	board_drawText(axis->name, &textPoint, currentProperties->font, hAlign, vAlign);
	
	if(axis->ticksCNT)
	{
		switch(orientation)
		 {
			 case HORIZONTAL:
				startPoint.y = getY(axis->position);
				hAlign = MIDDLE;
				vAlign = BOTTOM;
				break;
			 case UP:
				startPoint.x = getX(axis->position);
				hAlign = LEFT;
				vAlign = CENTER;
				break;
		 }	
		int i;	
		for(i = 0; i < axis->ticksCNT; i ++)
		{
			TickType* tickType = *(axis->tickTypes + i);
			char* text;
			 if(tickType->showText)
			 {
				 text = malloc(tickType->textMaxLength * sizeof(char));
			 }
			 switch(orientation)
			 {
				 case HORIZONTAL:
					endPoint.y = startPoint.y + tickType->size;
					textPoint.y = endPoint.y + currentProperties->textPadding;
					break;
				 case UP:
					endPoint.x = startPoint.x - tickType->size;
					textPoint.x = endPoint.x - currentProperties->textPadding;
					break;
			 }		 
			double currentTickData;
			for(currentTickData = axis->minData; currentTickData <= axis->maxData; currentTickData += tickType->padding)
			{
				 switch(orientation)
				 {
					 case HORIZONTAL:
						startPoint.x = getX(currentTickData);
						endPoint.x = startPoint.x;
						textPoint.x = endPoint.x;
						break;
					 case UP:
						startPoint.y = getY(currentTickData);
						endPoint.y = startPoint.y;
						textPoint.y = endPoint.y;
						break;
				 }
				 board_drawLine(&startPoint, &endPoint);
				 /*printf("Tick: ");
				 printPoint(&startPoint);
				 printf(" ");
				 printPoint(&endPoint);
				 printf("\n%f %f\n", currentTickData, axis->maxData);  */
				 if(tickType->showText)
				 {
					 int value = currentTickData;
					 if((((double)value) - currentTickData) != 0.0) 
					 { 
						snprintf(text, tickType->textMaxLength, "%f", currentTickData);
					 }
					 else
					 {
						 snprintf(text, tickType->textMaxLength, "%d", value);
					 }
					 board_drawText(text, &textPoint, currentProperties->font, hAlign, vAlign);
				 }
			}
		}
	}
}

void drawTitle()
{
	Point point;
	point.x = (currentProperties->size->width >> 1);
	point.y = (currentProperties->padding >> 1);
	
	board_drawText(currentProperties->name, &point, currentProperties->font, MIDDLE, BOTTOM);
}

void drawAgenda()
{
	unsigned short fontSize = currentProperties->font->size;
	unsigned short textPadding = currentProperties->textPadding;
	unsigned short linesCNT = currentDataLine + 1;
	unsigned short agendaHeight = (linesCNT + 1) * fontSize + linesCNT * textPadding;
	Point point;
	point.x = currentProperties->size->width - currentProperties->padding - currentProperties->agendaWidth;
	point.y = (currentProperties->size->height - agendaHeight) >> 1;
	board_setDrawColor(currentProperties->color);
	board_drawText(AGENDA_STR, &point, currentProperties->font, RIGHT, BOTTOM);

	Point symbolPoint;
	symbolPoint.x = point.x + (fontSize >> 1);
	Point textPoint;
	textPoint.x = point.x + fontSize + textPadding;
	unsigned short i;
	for (i = 0; i < linesCNT; i++)
	{
		point.y += fontSize + textPadding;

		symbolPoint.y = point.y + (fontSize >> 1);

		board_setDrawColor(dataLines[i]->color);
		drawShape(&symbolPoint, dataLines[i]->shape, dataLines[i]->size);

		textPoint.y = point.y;
		board_setDrawColor(currentProperties->color);
		board_drawText(dataLines[i]->name, &textPoint, currentProperties->font, RIGHT, BOTTOM);
	}
}

void drawShape(Point* centerPoint, Shape shape, unsigned short size)
{
	//printPoint(centerPoint);
	switch (shape)
	{
	case SQARE:
	{
		Dimensions dimensions;
		dimensions.width = size;
		dimensions.height = size;
		Point point;
		point.x = centerPoint->x - (size >> 1);
		point.y = centerPoint->y - (size >> 1);
		//printPoint(centerPoint);
		board_drawRectangle(&point, &dimensions, true);
		break;
	}
	case CROSS:
		board_drawX(centerPoint, size);
		break;
	}
	//printf("\n");
}

void getPoint(Point* point, double xData, double yData)
{
	/*printPoint(point);
	printf("1\n");*/
	point->x = getX(xData);
	/*printPoint(point);
	printf("2\n");*/
	point->y = getY(yData);
	/*printPoint(point);
	printf("3\n");*/
}

unsigned short getX(double xData)
{
	//printf("getX %d %f %f %f\n", chartStart.x, xData, currentProperties->xAxis->minData, scaleX);
	return chartStart.x + (xData - currentProperties->xAxis->minData) / scaleX;
}

unsigned short getY(double yData)
{
	//printf("getY %d %f %f %f\n", chartStart.y, yData, currentProperties->yAxis->minData, scaleY);
	return chartStart.y - (yData - currentProperties->yAxis->minData) / scaleY;
}


