#include "driver/gpio/gpio.h"
#include "driver/timer/systemTimer.h"
#include "driver/frameBuffer/frameBuffer.h"
#include "driver/drawing/drawing.h"

void kernelStart(void)
{
	setGpioFunction(16, 1);
	struct FrameBufferDescription* fb = initialiseFrameBuffer(1024, 768, 16);
	while(fb == 0)
	{
		setGpio(16, 0);
		wait(200000);
		setGpio(16, 1);
		wait(200000);

	}
	setGpio(16, 0);
	setGraphicsAddress(fb);
	int i = 5;
	int j = 5;
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
