#include "driver/frameBuffer/frameBuffer.h"
#include "driver/drawing/drawing.h"

#ifndef CHAR_SIZE

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

#define CHAR_SIZE
#endif

#ifndef FB_WIDTH
#define FB_WIDTH 1024
#define FB_HEIGHT 768
#endif

unsigned short charX = 0;
unsigned short charY = 0;

void printChar(char c)
{
	drawCharacter(c, charX, charY);
	charX += CHAR_WIDTH;
	while(charX >= FB_WIDTH)
	{
		charX -= FB_WIDTH;
		charY += CHAR_HEIGHT;
	}
	while(charY >= FB_HEIGHT)
	{
		charY -= FB_HEIGHT;
	}
}

void newLine(void)
{
	if(charY == FB_HEIGHT - 1)
	{
		charY = 0;
	} else {
		charY += CHAR_HEIGHT;
	}
	charX = 0;
}

void print(char* c)
{
	while(*c != '\0')
	{
		switch (*c)
		{
			case '\n' :
				newLine();
				break;
			default:
				printChar(*c);
		}
		c++;
	}
}

