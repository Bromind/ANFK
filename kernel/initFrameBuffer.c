#include "driver/frameBuffer.h"
#include "driver/drawing.h"

#ifndef CHAR_SIZE

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

#define CHAR_SIZE
#endif

struct FrameBufferDescription* fbInfo = 0;
unsigned short charX = 0;
unsigned short charY = 0;

void printChar(char c)
{
	drawCharacter(c, charX, charY);
	charX += CHAR_WIDTH;
	if(charX > FB_WIDTH)
	{
		charY += CHAR_HEIGHT;
	}
	charX %= FB_WIDTH;
	charY %= FB_HEIGHT;
}
