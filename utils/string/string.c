

int memcpy(void* original, void* destination, unsigned int length)
{
	int i = 0;
	for(; i < length ; i++)
	{
		*(char*)(destination+i) = *(char*) (original+i);
	}
	return i;
}

int stringCmp(char* s1, char* s2, unsigned int length)
{
	int toReturn = 1;
	length--;
	for(; length && toReturn; length--)
	{
		toReturn &= (*(s1+length) == *(s2+length));
	}
	return toReturn;
}
