#include "FileTool.h"
#include <stdlib.h>
#define PAIR_DELIMER "="

FILE* file_open(char* fileURL, char* mode)
{
	FILE *fp = fopen(fileURL, mode);
	if (!fp)
	{
		printf("Cannot open file %s\n", fileURL);
	}

	return fp;
}

int file_readIntPair(int* value, char* key, FILE* fp)
{
	int res = fscanf(fp, getKeyValueFormat(key,"%d"), value);
	if (res)
		return 0;

	printf("Cannot read int property %s\n", key);
	return 1;
}

int file_readSeveralIntPairs(unsigned short count, int** values, char** keys, FILE *fp)
{
	unsigned short result = count;
	int i;
	for (i = 0; i < count; i++)
	{
		char* key = *(keys + i);
		if (file_readIntPair(*(values + i), key, fp))
		{
			result--;
		}
	}

	return result;
}

int file_readFloatPair(float* value, char* key, FILE* fp)
{
	int res = fscanf(fp, getKeyValueFormat(key, "%f"), value);
	if (res)
		return 0;

	printf("Cannot read float property %s\n", key);
	return 1;
}

int file_readSeveralFloatPairs(unsigned short count, float** values, char** keys, FILE *fp)
{
	unsigned short result = count;
	int i;
	for (i = 0; i < count; i++)
	{
		char* key = *(keys + i);
		if (file_readFloatPair(*(values + i), key, fp))
		{
			result--;
		}
	}

	return result;
}

int file_readStringPair(char* value, char* key, FILE* fp)
{

	int res = fscanf(fp, getKeyValueFormat(key, "%s"), value);
	if (res)
		return 0;

	printf("Cannot read string property %s\n", key);
	return 1;
}

int file_readSeveralStringPairs(unsigned short count, char** values, char** keys, FILE *fp)
{
	unsigned short result = count;
	int i;
	for (i = 0; i < count; i++)
	{
		char* key = *(keys + i);
		if (file_readStringPair(*(values + i), key, fp))
		{
			result--;
		}
	}

	return result;
}

char* getKeyValueFormat(char* key, char* valueFormat)
{
	char* buff = malloc(sizeof(char) * BUFFER_SIZE);
	snprintf(buff, BUFFER_SIZE, "%s%s%s\n", key, PAIR_DELIMER, valueFormat);
	return buff;
}