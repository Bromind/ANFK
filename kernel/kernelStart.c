#include "driver/gpio/gpio.h"
#include "driver/timer/systemTimer.h"
#include "driver/frameBuffer/frameBuffer.h"
#include "driver/drawing/drawing.h"

#include "initFrameBuffer.h"

#include "logger.h"
#include "test.h"

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

/*
 *	Initialise a frame buffer, if initialisation fail, make ACT led blink fast.
 *	After initialisation, draw a 5 pixels square and make ACT led blink slow.
 */
void kernelStart(struct FrameBufferDescription* fb)
{
	setGpioFunction(16, 1);
	
	LOG("Hello World !");
	newLine();
	LOG("-------------");
	newLine();
	newLine();
	LOG("Project : Parallel kernel for RaspberryPi");
	newLine();
	newLine();
	LOG("git repository : https://github.com/Bromind/ANFK.git");
	LOG("starting kernel");
	runKernel();

}

