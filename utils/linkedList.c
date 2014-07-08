#include <stdlib.h>
#include "linkedList.h"


struct cell * createCell(void* element)
{
	struct cell * toReturn = malloc(sizeof(struct cell));
	toReturn->element = element;
	toReturn->next = toReturn;
	toReturn->previous = toReturn;
	return toReturn;
}

struct linkedList * newList(void)
{
	struct linkedList * toReturn = malloc(sizeof(struct linkedList));
	toReturn->head = NULL;
	toReturn->size = 0;
	return toReturn;
}

int isEmpty(struct linkedList * list)
{
	return list->size == 0;
}

int size(struct linkedList * list)
{
	return list->size;
}

struct linkedList * insert(struct linkedList * list, void* element)
{
	struct cell * newCell = createCell(element);
	if(list->size == 0)
	{
		list->head = newCell;
	} else {
		newCell->next = list->head;
		newCell->previous = newCell->next->previous;
		newCell->previous->next = newCell;
		list->head->previous = newCell;

		list->head = newCell;
	}
	list->size++;
	return list;
}

struct linkedList * rotateForward(struct linkedList * list)
{
	list->head = list->head->next;
	return list;
}

/* Remove the element at given cell from the list : 
   free the cell, not the element 
   return the element at given cell */
void* removeCell(struct linkedList * list, struct cell * cell)
{
	if(list->head == cell)
	{
		if(cell->next == cell)
		{
			list->head = NULL;
		} else {
			list->head = cell->next;
		}
	}
	void* element = cell->element;
	cell->next->previous = cell->previous;
	cell->previous->next = cell->next;
	free(cell);
	list->size--;
	return element;
}

void freeList(struct linkedList * list)
{
	free(list);
}

struct cell * getCellAtIndex(struct cell * cell, int index)
{
	struct cell * toReturn = NULL;
	if(index == 0)
		toReturn = cell;
	if(index > 0)
		toReturn = getCellAtIndex(cell->next, index - 1);
	if(index < 0)
		toReturn = getCellAtIndex(cell->previous, index + 1);
	return toReturn;
}

struct cell * getIndex(struct linkedList * list, int index)
{
	return getCellAtIndex(list->head, index);
}

