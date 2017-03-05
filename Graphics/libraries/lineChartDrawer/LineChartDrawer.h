#ifndef LINE_CHART_DRAWER_H
#define LINE_CHART_DRAWER_H

#include "../drawBoard/DrawBoard.h"

typedef enum 
{
	SQARE,
	CROSS 
} Shape;

typedef struct
{
	char* name;
	Shape shape;
	Color* color;
	unsigned short size;
}LineData;

typedef struct
{
	double padding;
	unsigned short size;
	bool showText;
	unsigned short textMaxLength;
}TickType;

typedef struct
{
	char* name;
	double maxData;
	double minData;
	double position;
	unsigned short ticksCNT;
	TickType** tickTypes;
}Axis;

typedef struct
{
	char* name;
	Dimensions* size;
	unsigned short padding;
	Color* backgroundColor;
	Color* color;
	Axis* xAxis;
	Axis* yAxis;
	unsigned short agendaWidth;
	Font* font;
	unsigned short textPadding;
}LineChartProperties;

int chart_initChart(LineChartProperties* properties);
int chart_defineLineData(LineData* lineData);
int chart_putData(double xData, double yData);
int chart_showChart();
int chart_waitAndClose();

#endif