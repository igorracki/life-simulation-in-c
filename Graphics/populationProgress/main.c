#include "../libraries/lineChartDrawer/LineChartDrawer.h"
#include "../libraries/tools/FileTool.h"
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define INPUT_FORMAT "<data_file> <config_file> [<threads_count>]"
#define CHART_TITLE "Population overflow"
#define X_AXIS_NAME "Tacts"
#define Y_AXIS_NAME "Population"
#define MAIN_TICK_SIZE 20
#define SEC_TICK_SIZE 10
#define MAX_TICK_TEXT_LEN 10
#define TEXT_PADDING 10

LineChartProperties* CHART_PROPERTIES;
unsigned short POINTS_SIZE;
unsigned short LINES_COLORS_CNT;
Color** LINES_COLORS;
char* DATA_FILE;
char* CONFIG_FILE;
unsigned short LINES_COUNT;
unsigned long LINES_LENGTH;
FILE* FP;

unsigned short THREAD_COUNT = 1;
unsigned long READ_DATA_COUNTER = 0;
unsigned short THREADS_AT_BARRIER_COUNTER = 0;
unsigned short READ_LINES = 0;
sem_t DATA_CNT_SEM;
sem_t BARRIER_SEM;
sem_t BARRIER_CNT_SEM;

void parseConfiguration();
void readInputArguments(int argc, char *argv[]);
void getLineColor(Color** color);
void parseDataFile_serial();
void parseDataFile_parallel();
void readNextData(int* data);
void parseLineHeadData();

void* threadWork(void* args);

int main(int argc, char *argv[])
{
	readInputArguments(argc, argv);
	parseConfiguration();

	FP = file_open(DATA_FILE, READ);
	if (!FP)
		exit(0);

	int buff;
	fscanf(FP, "%d\n", &buff);
	LINES_COUNT = buff;
	printf("Lines count: %d\n", LINES_COUNT);
	fscanf(FP, "%d\n", &buff);
	LINES_LENGTH = buff;
	printf("Lines length: %d\n", LINES_LENGTH);
	CHART_PROPERTIES->xAxis->maxData = LINES_LENGTH;

	chart_initChart(CHART_PROPERTIES);

	printf("Lets parse some data :)\n");
	clock_t begin = clock();
	if (THREAD_COUNT < 2)
	{
		//printf("Lines count: %d\n", LINES_COUNT);
		parseDataFile_serial();
	}
	else
	{
		parseDataFile_parallel();
	}
	printf("Parsing time: %fs\n", (double)(clock() - begin) / CLOCKS_PER_SEC);

	fclose(FP);

	//printf("Lets draw some chart.\n");
	chart_showChart();
	//printf("Lets wait.\n");
	chart_waitAndClose();
}

void parseDataFile_serial()
{
	int j = LINES_COUNT;
	char chBuff;
	//while(j --)
	while(LINES_COUNT --)
	{
		printf("Cycle %d begin!\n", j);
		//char** lineName = malloc(sizeof(char*));
		//if (!fscanf(fp, "%s\n", *(lineName)))
		
		parseLineHeadData();
		int buff;
		int i;
		for (i = 0;  i < LINES_LENGTH; i++)
		{
			readNextData(&buff);
			chart_putData(i, buff);
		}
		//fscanf(FP, "%s\n", &chBuff);
		printf("Next %d, please.\n",j);
		
	}
}

void parseDataFile_parallel()
{
	sem_init(&DATA_CNT_SEM, 0, 1);
	sem_init(&BARRIER_SEM, 0, 0);
	sem_init(&BARRIER_CNT_SEM, 0, 1);

	pthread_t threads[THREAD_COUNT];
	//pthread_t* threads = malloc(THREAD_COUNT * sizeof(pthread_t));
	parseLineHeadData();

	int i;
	for (i = 0; i < THREAD_COUNT; i++)
	{
		printf("Init %d thread.\n", i);
		pthread_create(&threads[i], NULL, threadWork, (void* )i);
	}
	for (i = 0; i < THREAD_COUNT; i++)
	{
		pthread_join(threads[i], NULL);
	}

	//free(threads);

	sem_destroy(&DATA_CNT_SEM);
	sem_destroy(&BARRIER_SEM);
	sem_destroy(&BARRIER_CNT_SEM);

}

void* threadWork(void* args)
{
	int rank = (int)args;
	int i;
	int buff;
	char chBuff;
	while (READ_LINES < LINES_COUNT)
	{
		sem_wait(&DATA_CNT_SEM);
		if (READ_DATA_COUNTER < LINES_LENGTH)
		{
			readNextData(&buff);
			i = READ_DATA_COUNTER;
			READ_DATA_COUNTER++;
			sem_post(&DATA_CNT_SEM);

			//printf("%d: Data read: %d\n", rank, buff);
			chart_putData(i, buff);
			//printf("%d: Data displayed!\n", rank);
		}
		else
		{
			sem_post(&DATA_CNT_SEM);
			sem_wait(&BARRIER_CNT_SEM);
			THREADS_AT_BARRIER_COUNTER++;
			if (THREADS_AT_BARRIER_COUNTER == THREAD_COUNT)
			{
				THREADS_AT_BARRIER_COUNTER = 0;
				sem_post(&BARRIER_CNT_SEM);

				READ_LINES++;
				READ_DATA_COUNTER = 0;

				if (READ_LINES < LINES_COUNT)
				{
					//fscanf(FP, "%s\n", &chBuff);
					parseLineHeadData();
				}

				for (i = 0; i < THREAD_COUNT; i++)
					sem_post(&BARRIER_SEM);
			}
			else
			{
				sem_post(&BARRIER_CNT_SEM);
				sem_wait(&BARRIER_SEM);
			}
		}
	}
}

void parseLineHeadData()
{
	//char* lineName = malloc(sizeof(char) * BUFFER_SIZE);
	//fscanf(fp, "\n%s\n", lineName);

	//printf("Read name: %s\n", lineName);
	LineData* lineData = malloc(sizeof(LineData));
	//lineData->name = *(lineName);
	lineData->name = malloc(sizeof(char) * BUFFER_SIZE);
	fscanf(FP, "%s\n", lineData->name);
	//strcpy(lineData->name, lineName);
	//printf("Copied name: %s\n", lineData->name);
	//lineData->color = malloc(sizeof(Color));
	getLineColor(&(lineData->color));
	lineData->shape = SQARE;
	lineData->size = POINTS_SIZE;
	chart_defineLineData(lineData);
}

void readNextData(int* data)
{
	fscanf(FP, "%d", data);
	//printf("Read data: %d\n", buff);
}

void getLineColor(Color** color)
{
	static unsigned short i = 0;
	*(color) = *(LINES_COLORS + i);
	i = (i + 1) % LINES_COLORS_CNT;
}

void parseConfiguration()
{
	FILE* fp = file_open(CONFIG_FILE, READ);
	if (!fp)
		exit(0);
	int buff;

	int missingProperties = 0;
	CHART_PROPERTIES = malloc(sizeof(LineChartProperties));
	CHART_PROPERTIES->name = CHART_TITLE;

	CHART_PROPERTIES->size = malloc(sizeof(Dimensions));
	missingProperties += file_readIntPair(&buff, "windowWidth", fp);
	CHART_PROPERTIES->size->width = buff;
	missingProperties += file_readIntPair(&buff, "windowHeight", fp);
	CHART_PROPERTIES->size->height = buff;

	missingProperties += file_readIntPair(&buff, "padding", fp);
	CHART_PROPERTIES->padding = buff;
	missingProperties += file_readIntPair(&buff, "agendaWidth", fp);
	CHART_PROPERTIES->agendaWidth = buff;
	missingProperties += file_readIntPair(&buff, "pointsSize", fp);
	POINTS_SIZE = buff;

	CHART_PROPERTIES->font = malloc(sizeof(Font));
	missingProperties += file_readIntPair(&buff, "fontSize", fp);
	(CHART_PROPERTIES->font->size) = buff;
	CHART_PROPERTIES->font->URL = malloc(sizeof(char) * BUFFER_SIZE);
	missingProperties += file_readStringPair(CHART_PROPERTIES->font->URL, "fontURL", fp);

	CHART_PROPERTIES->xAxis = malloc(sizeof(Axis));
	CHART_PROPERTIES->xAxis->name = X_AXIS_NAME;
	CHART_PROPERTIES->xAxis->position = 0;
	CHART_PROPERTIES->xAxis->minData = 0;

	CHART_PROPERTIES->yAxis = malloc(sizeof(Axis));
	CHART_PROPERTIES->yAxis->name = Y_AXIS_NAME;
	CHART_PROPERTIES->yAxis->position = 0;
	CHART_PROPERTIES->yAxis->minData = 0;
	missingProperties += file_readIntPair(&buff, "maxY", fp);
	CHART_PROPERTIES->yAxis->maxData = buff;

	TickType* xMainTickType = malloc(sizeof(TickType));
	missingProperties += file_readIntPair(&buff, "xMainTickStep", fp);
	xMainTickType->padding = buff;
	xMainTickType->size = MAIN_TICK_SIZE;
	xMainTickType->showText = true;
	xMainTickType->textMaxLength = MAX_TICK_TEXT_LEN;

	CHART_PROPERTIES->xAxis->tickTypes = malloc(sizeof(TickType*) * 2);
	CHART_PROPERTIES->xAxis->ticksCNT = 1;
	*(CHART_PROPERTIES->xAxis->tickTypes) = xMainTickType;

	TickType* yMainTickType = malloc(sizeof(TickType));
	missingProperties += file_readIntPair(&buff, "yMainTickStep", fp);
	yMainTickType->padding = buff;
	yMainTickType->size = MAIN_TICK_SIZE;
	yMainTickType->showText = true;
	yMainTickType->textMaxLength = MAX_TICK_TEXT_LEN;

	CHART_PROPERTIES->yAxis->tickTypes = malloc(sizeof(TickType*) * 2);
	CHART_PROPERTIES->yAxis->ticksCNT = 1;
	*(CHART_PROPERTIES->yAxis->tickTypes) = yMainTickType;

	if (!file_readIntPair(&buff, "secTickStepDivisor", fp) && buff)
	{
		TickType* xSecTickType = malloc(sizeof(TickType));
		xSecTickType->padding = xMainTickType->padding / buff;
		xSecTickType->size = SEC_TICK_SIZE;
		xSecTickType->showText = false;
		CHART_PROPERTIES->xAxis->ticksCNT = 2;
		*(CHART_PROPERTIES->xAxis->tickTypes + 1) = xSecTickType;

		TickType* ySecTickType = malloc(sizeof(TickType));
		ySecTickType->padding = yMainTickType->padding / buff;
		ySecTickType->size = SEC_TICK_SIZE;
		ySecTickType->showText = false;
		CHART_PROPERTIES->yAxis->ticksCNT = 2;
		*(CHART_PROPERTIES->yAxis->tickTypes + 1) = ySecTickType;
	}
	
	CHART_PROPERTIES->backgroundColor = malloc(sizeof(Color));
	missingProperties += readColorPairFromFileStream(CHART_PROPERTIES->backgroundColor, "backgroundColor", fp);

	CHART_PROPERTIES->color = malloc(sizeof(Color));
	missingProperties += readColorPairFromFileStream(CHART_PROPERTIES->color, "textColor", fp);

	CHART_PROPERTIES->textPadding = TEXT_PADDING;

	int res = file_readIntPair(&buff, "linesColors", fp);
	if (!res)
	{
		if (buff > 0)
		{
			LINES_COLORS_CNT = buff;
			LINES_COLORS = malloc(sizeof(Color*) * LINES_COLORS_CNT);
			int i;
			for (i = 0; i < LINES_COLORS_CNT; i++)
			{
				*(LINES_COLORS + i) = malloc(sizeof(Color));
				missingProperties += readColorFromFileStream(*(LINES_COLORS + i), fp);
			}
		}
		else
		{
			printf("At least one color needed!\n");
			missingProperties++;
		}
	}
	else
	{
		missingProperties += res;
	}

	fclose(fp);
	if (missingProperties)
		exit(1);
}

void readInputArguments(int argc, char *argv[])
{
	if (argc >= 3 && argc <= 4)
	{
		DATA_FILE = argv[1];
		CONFIG_FILE = argv[2];
		if (argc == 4)
		{
			int buff = atoi(argv[3]);
			if (buff < 1)
			{
				printf("Thread count must be a N value!\n");
			}
			else
			{
				THREAD_COUNT = buff;
				return;
			}
		}
		else
		{
			return;
		}
	}

	printf("Required input arguments: %s\n", INPUT_FORMAT);
	exit(1);
}