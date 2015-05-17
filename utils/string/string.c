

int memcpy(void* original, void* destination, unsigned int length)
{
	int i = 0;
	for(; i < length ; i++)
	{
		*(char*)(destination+i) = *(char*) (original+i);
	}
	return i;
}
