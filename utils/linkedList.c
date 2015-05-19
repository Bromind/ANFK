#ifndef ALLOCATION_TABLE_H
#include "../kernel/mem/allocationTable.h"
#define ALLOCATION_TABLE_H
#endif

#ifndef LINKEDLIST_H
#include "linkedList.h"
#define LINKEDLIST_H
#endif

#define LINKEDLIST_ID 2
#define NULL 0

struct cell * createCell(void* element)
{
	struct cell * toReturn = 
		allocateMemory(sizeof(struct cell), LINKEDLIST_ID);
	toReturn->element = element;
	toReturn->next = toReturn;
	toReturn->previous = toReturn;
	return toReturn;
}

struct linkedList * newList(void)
{
	struct linkedList * toReturn = 
		allocateMemory(sizeof(struct linkedList), LINKEDLIST_ID);
	toReturn->head = NULL;
	toReturn->size = 0;
	return toReturn;
}

int isEmpty(struct linkedList * list)
{
	return list->size == 0;
}

unsigned int size(struct linkedList * list)
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

struct linkedList * insertAtEnd(struct linkedList * list, void* element)
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
	freeMemory(cell, LINKEDLIST_ID);
	list->size--;
	return element;
}

void freeList(struct linkedList * list)
{
	freeMemory(list, LINKEDLIST_ID);
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

