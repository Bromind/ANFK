#include "driver/gpio/gpio.h"
#include "driver/timer/systemTimer.h"

void kernelStart(void)
{
	setGpioFunction(16, 1);
	while(1)
	{
		setGpio(16, 0);
		wait(1000000);
		setGpio(16, 1);
		wait(1000000);
	}
}
