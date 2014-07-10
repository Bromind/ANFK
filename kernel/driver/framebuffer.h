struct frameBuffer
{
	int physicalWidth;
	int physicalHeight;
	int virtualWidth;
	int virtualHeight;
	int gpuPitch;
	int x;
	int y;
	int gpuPointer;
	int gpuSize;
};


extern struct frameBuffer* initializeFrameBuffer(int width, int height, int bitDepth);
