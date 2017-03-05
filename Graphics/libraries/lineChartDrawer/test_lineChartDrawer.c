#include "LineChartDrawer.h"
#include <stdlib.h>

#define FONT_URL "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-BI.ttf"

void initChart();

int main()
{
	initChart();

	LineData* lineData = malloc(sizeof(LineData));
	lineData->name = "1stLine";
	Color* blueColor = malloc(sizeof(Color));
	blueColor->red = 0x00;
	blueColor->green = 0x00;
	blueColor->blue = 0xff;
	lineData->color = blueColor;
	lineData->shape = SQARE;
	lineData->size = 10;

	chart_defineLineData(lineData);
	chart_putData(5000, 0.003);
	chart_putData(6000, 0.003);
	chart_putData(7000, 0.003);

	LineData* lineData2 = malloc(sizeof(LineData));
	lineData2->name = "2ndLine";
	Color* greenColor = malloc(sizeof(Color));
	greenColor->red = 0x00;
	greenColor->green = 0xff;
	greenColor->blue = 0x00;
	lineData2->color = greenColor;
	lineData2->shape = CROSS;
	lineData2->size = 10;

	chart_defineLineData(lineData2);
	chart_putData(5000, 0.01);
	chart_putData(6000, 0.01);
	chart_putData(7000, 0.01);

	
	chart_showChart();
	chart_waitAndClose();
	
	return 0;
}

void initChart()
{

	LineChartProperties* chartProperties = malloc(sizeof(LineChartProperties));
	chartProperties->name = "testChart";
	
	Dimensions* dimensions = malloc(sizeof(Dimensions));
	dimensions->width = 1000;
	dimensions->height = 800;
	chartProperties->size = dimensions;

	chartProperties->padding = 100;

	Color* blackColor = malloc(sizeof(Color));
	blackColor->red = 0;
	blackColor->green = 0;
	blackColor->blue = 0;
	chartProperties->backgroundColor = blackColor;

	Color* whiteColor = malloc(sizeof(Color));
	whiteColor->red = 0xff;
	whiteColor->green = 0xff;
	whiteColor->blue = 0xff;
	chartProperties->color = whiteColor;

	Axis* xAxis = malloc(sizeof(Axis));
	xAxis->name = "xAxis";
	xAxis->maxData = 10000;
	xAxis->minData = 0;
	xAxis->position = 0;

	TickType* tickType1 = malloc(sizeof(TickType));
	tickType1->padding = 1000;
	tickType1->size = 20;
	tickType1->showText = true;
	tickType1->textMaxLength = 6;

	TickType* tickType2 = malloc(sizeof(TickType));
	tickType2->padding = 200;
	tickType2->size = 10;
	tickType2->showText = false;

	TickType** types = malloc(sizeof(TickType*) * 2);
	*types = tickType1;
	*(types + 1) = tickType2;
	xAxis->ticksCNT = 2;
	xAxis->tickTypes = types;

	Axis* yAxis = malloc(sizeof(Axis));
	yAxis->name = "yAxis";
	yAxis->maxData = 0.02;
	yAxis->minData = 0;
	yAxis->position = 0;

	TickType* tickType3 = malloc(sizeof(TickType));
	tickType3->padding = 0.003;
	tickType3->size = 20;
	tickType3->showText = true;
	tickType3->textMaxLength = 6;

	TickType** type = malloc(sizeof(TickType*) * 1);
	*type = tickType3;
	yAxis->ticksCNT = 1;
	yAxis->tickTypes = type;

	chartProperties->xAxis = xAxis;
	chartProperties->yAxis = yAxis;

	chartProperties->agendaWidth = 100;

	Font* font = malloc(sizeof(Font));
	font->URL = FONT_URL;
	font->size = 20;
	chartProperties->font = font;
	chartProperties->textPadding = 5;

	chart_initChart(chartProperties);
}