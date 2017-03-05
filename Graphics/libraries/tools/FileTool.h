#ifndef FILE_TOOL_H
#define FILE_TOOL_H

#include<stdio.h>

#define READ "r"
#define WRITE "w"

#define BUFFER_SIZE 255

FILE* file_open(char* fileURL, char* mode);
int file_readIntPair(int* value, char* key, FILE* fp);
int file_readSeveralIntPairs(unsigned short count, int** values, char** keys, FILE *fp);
int file_readFloatPair(float* value, char* key, FILE* fp);
int file_readSeveralFloatPairs(unsigned short count, float** values, char** keys, FILE *fp);
int file_readStringPair(char* value, char* key, FILE* fp);
int file_readSeveralStringPairs(unsigned short count, char** values, char** keys, FILE *fp);

char* getKeyValueFormat(char* key, char* valueFormat);
#endif // !FILE_TOOL_H