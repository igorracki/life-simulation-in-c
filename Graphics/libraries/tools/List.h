#ifndef LIST_H
#define LIST_H

#include<semaphore.h>

typedef struct
{
	struct ListItem* next;
	struct ListItem* previous;
	void* item;
} ListItem;

typedef struct
{
	ListItem* start;
	ListItem* end;
	sem_t* writeLock;
	unsigned short length;
} List;

int list_init(List* list);
int list_addItem(List* list, void* item);

#endif // !LIST_H
