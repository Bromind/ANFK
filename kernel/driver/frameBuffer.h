struct FrameBufferDescription
{
	u32 physicalWidth;
	u32 physicalHeight;
	u32 virtualWidth;
	u32 virtualHeight;
	u32 gpuPitch;
	u32 x;
	u32 y;
	void* gpuPou32er;
	u32 gpuSize;
};


extern struct FrameBufferDescription* initializeFrameBuffer(u32 width, 
		u32 height, u32 bitDepth);
