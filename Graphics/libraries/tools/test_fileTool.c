#include "FileTool.h"
#include <stdlib.h>

#define TEST_FILE "chartConfig.txt"

int main()
{
	FILE* fp = file_open(TEST_FILE, READ);
	if (!fp)
		exit(0);

	char* labels[] = {"windowWidth", "windowHeight", "padding", "agendaWidth", "pointsSize", "fontSize"};
	int i;
	for (i = 0; i < 6; i++)
	{
		int n;
		file_readIntPair(&n, labels[i], fp);
		printf("integer: %d\n", n);
	}

	char str[255];
	file_readStringPair(str, "fontURL", fp);
	printf("string: %s\n", str);

	char* labels2[] = { "maxX", "maxY", "mainTickStep", "secTickStep"};

	for (i = 0; i < 4; i++)
	{
		float f;
		file_readFloatPair(&f, labels2[i], fp);
		printf("float: %f\n", f);
	}

	fclose(fp);
	return 0;
}