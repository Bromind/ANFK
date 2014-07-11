#include "framebuffer.h"
#include "drawing.h"

int main(void)
{
	initializeFrameBuffer(1024, 768, 16);
	setForeColour(0);
	for(int i = 0 ; i < 1024 ; i++)
	{
		for(int j = 0 ; j < 768 ; j++)
		{
			drawPixel(i, j);
		}
	}
	setForeColour(4096);
	drawLine(0, 0, 500, 500);
	
}
