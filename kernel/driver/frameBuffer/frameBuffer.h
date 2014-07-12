# ifndef FB_DESCR
struct FrameBufferDescription
{
	int physicalWidth;
	int physicalHeight;
	int virtualWidth;
	int virtualHeight;
	int gpuPitch;
	int bitDepth;
	int x;
	int y;
	void* gpuPointer;
	int gpuSize;
};
#define FB_DESCR
#endif


struct FrameBufferDescription* initializeFrameBuffer(int width, 
		int height, int bitDepth);
