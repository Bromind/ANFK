#include "driver/gpio/gpio.h"
#include "driver/timer/systemTimer.h"
#include "driver/frameBuffer/frameBuffer.h"
#include "driver/drawing/drawing.h"


/*
 *	Initialise a frame buffer, if initialisation fail, make ACT led blink fast.
 *	After initialisation, draw a 5 pixels square and make ACT led blink slow.
 */
void kernelStart(struct FrameBufferDescription* fb)
{
	setGpioFunction(16, 1);
	
	int i = 15;
	int j = 15;
	
	for(;i < 10 ; i++)
	{
		for(; j < 10 ; j++)
		{
			drawPixel(i, j);
		}
	}
	
	while(1){
		setGpio(16, 0);
		wait(500000);
		setGpio(16, 1);
		wait(500000);
	}
}

void blinkFast(void)
{
	while(1)
	{
		setGpio(16, 0);
		wait(200000);
		setGpio(16, 1);
		wait(200000);
	}
}

void blinkSlow(void)
{
	while(1)
	{
		setGpio(16, 0);
		wait(800000);
		setGpio(16, 1);
		wait(200000);
	}
}
