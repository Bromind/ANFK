#include "frameBuffer.h"
#include "../mailbox/mailbox.h"

struct FrameBufferDescription fbInfo = 
		{1024, 768, 1024, 768, 0, 24, 0, 0, 0, 0};

struct FrameBufferDescription* 
initialiseFrameBuffer(int width, int height, int bitDepth)
{
	if(width > 4096 || height > 4096 || bitDepth > 32)
	{
		return 0;
	}
	fbInfo.physicalWidth = width;
	fbInfo.virtualWidth = width;

	fbInfo.physicalHeight = height;
	fbInfo.virtualHeight = height;

	fbInfo.bitDepth = bitDepth;

	mailboxWrite((int) (0x40000000 + &fbInfo), 1);
	 /* If we read 0 from the mailbox, that is the initialisation of
	  the framebuffer is successfull, otherwise, it's a failure*/
	if(mailboxRead(1) == 0)
	{
		return &fbInfo;
	} else {
		return 0;
	}
}
