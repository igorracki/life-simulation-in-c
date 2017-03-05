#include "../libraries/mapDrawer/MapDrawer.h"
#include "../libraries/tools/FileTool.h"
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#define INPUT_FORMAT "<data_file> <config_file> [<threads_count>]"
#define TEXT_PADDING 10

MapProperties* MAP_PROPERTIES;
unsigned short POINTS_SIZE;
unsigned short OBJECT_COLORS_CNT;
Color** OBJECT_COLORS;
char* DATA_FILE;
char* CONFIG_FILE;
unsigned short LINES_COUNT;
unsigned short LINES_LENGTH;
unsigned long DATA_TO_BE_READ_CNT;
FILE* FP;

unsigned short THREAD_COUNT = 1;
unsigned long READ_DATA_COUNTER = 0;
sem_t DATA_CNT_SEM;

void parseConfiguration();
void readInputArguments(int argc, char *argv[]);
void getObjectColor(Color** color);
void parseDataFile_serial();
void parseDataFile_parallel();
bool readNextData(char* data);

void* threadWork(void* args);

int main(int argc, char *argv[])
{
	printf("Parsing args.\n");
	readInputArguments(argc, argv);
	printf("Parsing configuration.\n");
	parseConfiguration();

	FP = file_open(DATA_FILE, READ);
	if (!FP)
		exit(0);

	printf("Parsing header data.\n");
	int buff;
	fscanf(FP, "%d", &buff);
	int i;
	unsigned short objectTypesCount = buff;
	MapObjectType** objectTypes = malloc(sizeof(MapObjectType*) * buff);
	for (i = 0; i < objectTypesCount; i++)
	{
		MapObjectType* mapObjectType = malloc(sizeof(MapObjectType));
		mapObjectType->counter = 0;
		mapObjectType->name = malloc(sizeof(char) * BUFFER_SIZE);;
		fscanf(FP, "%s %c", mapObjectType->name, &(mapObjectType->mapChar));
		mapObjectType->displayColor = malloc(sizeof(Color));
		getObjectColor(&(mapObjectType->displayColor));
		*(objectTypes + i) = mapObjectType;
	}
	MAP_PROPERTIES->mapSize = malloc(sizeof(Dimensions));
	fscanf(FP, "%d", &buff);
	LINES_COUNT = buff;
	MAP_PROPERTIES->mapSize->height = buff;
	fscanf(FP, "%d", &buff);
	LINES_LENGTH = buff;
	MAP_PROPERTIES->mapSize->width = buff;
	DATA_TO_BE_READ_CNT = LINES_COUNT * LINES_LENGTH;

	printf("Map init.\n");
	map_init(MAP_PROPERTIES);
	for (i = 0; i < objectTypesCount; i++)
	{
		map_declareObjectType(*(objectTypes + i));
	}

	//printf("Lets parse some data :)\n");

	printf("Parsing data.\n");
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

	map_displayAndWait();
}

void parseDataFile_serial()
{
	Point* point = malloc(sizeof(Point));
	char currentData;
	bool eof;
	for (point->y = 0; point->y < LINES_COUNT; point->y++)
	{
		for (point->x = 0; point->x < LINES_LENGTH; point->x++)
		{
			eof = readNextData(&currentData);
			map_addObject(point, currentData);
			if (eof)
				return;
		}
	}
}

void parseDataFile_parallel()
{
	sem_init(&DATA_CNT_SEM, 0, 1);

	pthread_t threads[THREAD_COUNT];

	int i;
	for (i = 0; i < THREAD_COUNT; i++)
	{
		pthread_create(&threads[i], NULL, threadWork, (void*)i);
	}
	for (i = 0; i < THREAD_COUNT; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
}

void* threadWork(void* args)
{
	char currentData;
	bool eof;
	unsigned long currentDataIX;
	Point point;
	do
	{
		sem_wait(&DATA_CNT_SEM);
		currentDataIX = READ_DATA_COUNTER++;
		eof = readNextData(&currentData);
		sem_post(&DATA_CNT_SEM);

		if (currentDataIX >= DATA_TO_BE_READ_CNT)
			break;

		point.x = currentDataIX % LINES_LENGTH;
		point.y = currentDataIX / LINES_LENGTH;
		map_addObject(&point, currentData);
	} while (!eof);
}

bool readNextData(char* data)
{
	char readChar;
	bool eof = false;
	do
	{
		readChar = fgetc(FP);
		eof = feof(FP);
	} while (!eof && readChar == '\n');

	*data = readChar;

	return eof;
}

void getObjectColor(Color** color)
{
	static unsigned short i = 0;
	*(color) = *(OBJECT_COLORS + i);
	i++;
	if (i >= OBJECT_COLORS_CNT)
	{
		i = 0;
		printf("Note that color has been reset and you may see different objects with the same color");
	}
}

void parseConfiguration()
{
	FILE* fp = file_open(CONFIG_FILE, READ);
	if (!fp)
		exit(0);
	int buff;

	int missingProperties = 0;
	MAP_PROPERTIES = malloc(sizeof(MapProperties));
	MAP_PROPERTIES->title = DATA_FILE;

	MAP_PROPERTIES->windowPixelSize = malloc(sizeof(Dimensions));
	missingProperties += file_readIntPair(&buff, "windowWidth", fp);
	MAP_PROPERTIES->windowPixelSize->width = buff;
	missingProperties += file_readIntPair(&buff, "windowHeight", fp);
	MAP_PROPERTIES->windowPixelSize->height = buff;

	missingProperties += file_readIntPair(&buff, "padding", fp);
	MAP_PROPERTIES->padding = buff;
	missingProperties += file_readIntPair(&buff, "agendaWidth", fp);
	MAP_PROPERTIES->agendaWidth = buff;

	MAP_PROPERTIES->textFont = malloc(sizeof(Font));
	missingProperties += file_readIntPair(&buff, "fontSize", fp);
	(MAP_PROPERTIES->textFont->size) = buff;
	MAP_PROPERTIES->textFont->URL = malloc(sizeof(char) * BUFFER_SIZE);
	missingProperties += file_readStringPair(MAP_PROPERTIES->textFont->URL, "fontURL", fp);

	MAP_PROPERTIES->backgroundColor = malloc(sizeof(Color));
	missingProperties += readColorPairFromFileStream(MAP_PROPERTIES->backgroundColor, "backgroundColor", fp);

	MAP_PROPERTIES->textColor = malloc(sizeof(Color));
	missingProperties += readColorPairFromFileStream(MAP_PROPERTIES->textColor, "textColor", fp);

	MAP_PROPERTIES->textPadding = TEXT_PADDING;

	int res = file_readIntPair(&buff, "objectColors", fp);
	if (!res)
	{
		if (buff > 0)
		{
			OBJECT_COLORS_CNT = buff;
			OBJECT_COLORS = malloc(sizeof(Color*) * OBJECT_COLORS_CNT);
			int i;
			for (i = 0; i < OBJECT_COLORS_CNT; i++)
			{
				*(OBJECT_COLORS + i) = malloc(sizeof(Color));
				missingProperties += readColorFromFileStream(*(OBJECT_COLORS + i), fp);
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