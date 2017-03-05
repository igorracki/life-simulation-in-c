#include "List.h"
#include <stdlib.h>

int list_init(List * list)
{
	list->start = NULL;
	list->end = NULL;
	list->writeLock = malloc(sizeof(sem_t));
	sem_init(list->writeLock, 0, 1);
	list->length = 0;
	return 0;
}

int list_addItem(List* list, void* item)
{
	ListItem* listItem = malloc(sizeof(ListItem));
	listItem->item = item;
	listItem->next = NULL;
	listItem->previous = NULL;

	sem_wait(list->writeLock);
	if (list->start)
	{
		list->end->next = listItem;
		listItem->previous = list->end;
		list->end = listItem;
	}
	else
	{
		list->start = listItem;
		list->end = listItem;
	}
	list->length++;
	sem_post(list->writeLock);

	return 0;
}