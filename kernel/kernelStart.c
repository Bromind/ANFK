#ifndef GPIO_H
#include "driver/gpio/gpio.h"
#define GPIO_H
#endif

#ifndef TIMER_H
#include "driver/timer/systemTimer.h"
#define TIMER_H
#endif

#ifndef LOGGER_H
#include "logger.h"
#define LOGGER_H
#endif

#ifndef INIT_H
#include "init.h"
#define INIT_H
#endif

#ifndef PROCESS_MANAGER_H
#include "processManager/processManager.h"
#define PROCESS_MANAGER_H
#endif 

#ifndef PROCESS_DESCRIPTOR_H
#include "processManager/processDescriptor.h"
#define PROCESS_DESCRIPTOR_H
#endif

#ifndef INIT_SUBSYSTEMS_H
#include "initSubsystems.h"
#define INIT_SUBSYSTEMS_H
#endif


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
void kernelStart(void)
{
	setGpioFunction(16, 1);
	
	LOG("+----------------+\n");
	LOG("| K-FetOS / ANFK |\n");
	LOG("+----------------+\n");
	LOG("starting kernel\n");

	initSubsystems();
	struct cell* p1 = createProcess(&init, 0);
	start(p1);
	startScheduler();
}

