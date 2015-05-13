
struct allocationTable
{
	int pageSize;
	int numberbPage;
	void* beginPointer;
	int* processID;
}

void* allocateMemory(size_t size, int runningProcess)
{
	/* Do +1 avoid problems for rounding, 
	 * trade small memory versus cpu time
	 * */
	int numbPageNeeded = size/pageSize + 1;
	int pagesFound = 0;
	int remainingPages = numberbPage;

	while (remainingPages && pagesFound != numbPageNeeded)
	{
		if(*(processID+remainingPages-1))
		{
			pagesFound++;
		} else {
			pagesFound = 0;
		}
		remainingPages--;
	}
	if(pagesFound == numbPageNeeded)
	{
		for(;numbPageNeeded > 0 ; numbPageNeeded--)
		{
			*(processID+numbPageNeeded-1) = runningProcess;
		}
	}
}
