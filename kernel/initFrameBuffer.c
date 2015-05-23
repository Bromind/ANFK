#include "driver/frameBuffer/frameBuffer.h"
#include "driver/drawing/drawing.h"
#include "logger.h"
#include "driver/timer/systemTimer.h"
#include "../utils/maths/div.h"

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

void eraseScreen(void)
{
	LOG("Prepare to erase whole screen");
	wait(1000000);
	setForeColour(0);
	int i;
	for(i = 0 ; i < FB_HEIGHT ; i++)
	{
		int j;
		for(j = 0 ; j < FB_WIDTH ; j++)
		{
			drawPixel(i, j);
		}
	}
	setForeColour(0xFFFF);
	LOG("Screen erased");
}

void eraseScreenUp(void)
{
	LOG("Prepare to erase upper part of the screen");
	wait(1000000);
	setForeColour(0);
	int i;
	for(i = 0 ; i < div(FB_HEIGHT, 2) ; i++)
	{
		int j;
		for(j = 0 ; j < FB_WIDTH ; j++)
		{
			drawPixel(i, j);
		}
	}
	setForeColour(0xFFFF);
}

void eraseScreenDown(void)
{
	LOG("Prepare to erase lower part of the screen");
	wait(1000000);
	setForeColour(0);
	int i;
	for(i = div(FB_HEIGHT, 2) ; i < FB_HEIGHT ; i++)
	{
		int j;
		for(j = 0 ; j < FB_WIDTH ; j++)
		{
			drawPixel(i, j);
		}
	}
	setForeColour(0xFFFF);
}

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
	printChar(92);
	printChar('n');
	if(charY == div(FB_HEIGHT, 2) - 3) 
	{
		eraseScreenDown();
	}
	if(charY == FB_HEIGHT - 3)
	{
		eraseScreenUp();
	}
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

void printInt(int i)
{
	char string[11];
	string[0] = '0';
	string[1] = 'x';
	string[2] = (char) (i >> 28) & 0xF;
	string[3] = (char) (i >> 24) & 0xF;
	string[4] = (char) (i >> 20) & 0xF;
	string[5] = (char) (i >> 16) & 0xF;
	string[6] = (char) (i >> 12) & 0xF;
	string[7] = (char) (i >> 8) & 0xF;
	string[8] = (char) (i >> 4) & 0xF;
	string[9] = (char) i& 0xF;
	string[10] = '\0';
	int j = 2;
	for(; j < 10 ; j++)
	{
		string[j] = string[j] < 10 ? string[j] + 48 : string[j] + 55;
	}
	print(string);
}
