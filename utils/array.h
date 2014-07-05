struct DynamicArray
{
	int size;
	void** array;
};

struct DynamicArray * newArray(void);
int addElement(struct DynamicArray * array, void* element, int index);
void* getElement(struct DynamicArray * array, int index);
void freeElement(struct DynamicArray * array, int index);
void freeArray(struct DynamicArray * array);
