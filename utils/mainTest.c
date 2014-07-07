#include<stdio.h>
#include "array.h"
#include "linkedList.h"

void testArray(void);
void testLinkedlist();

int main(void)
{
	testArray();
	testLinkedlist();
	return 0;
}

void testArray(void)
{
	printf("=== Test array ===\n");
	struct DynamicArray * array = newArray();
	for(int i = 0 ; i < 5 ; i++)
	{
		addElement(array, &i, i);
		int* res = getElement(array, 0);
		printf("At index i : %d\n", *res);
	}
	freeArray(array);
}

void testLinkedlist()
{
	printf("=== Test linkedList ===\n");
	struct linkedList * list = newList();
	if(isEmpty(list))
	{
		printf("The list is empty\n");
	}
	
	int integers[] = {1, 2, 3};

	for(int i = 0 ; i < 3 ; i++)
	{
		insert(list, &integers[i]);
	}
	rotateForward(list);

	for(int i = 0 ; i < 3 ; i++)
	{
		struct cell* cell= getIndex(list, i);
		printf("At index %d ; %d\n", i, *(int*)cell->element);
	}
	while(list->head != NULL)
	{
		removeCell(list, getIndex(list, 0));
	}
	freeList(list);
}
