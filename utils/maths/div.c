
#include "../../kernel/logger.h"

unsigned int div(unsigned int dividand, unsigned int divisor)
{
	if(divisor == 0)
	{
		LOG("Error, division by 0");
		return 0;
	}
	unsigned int i = 0;
	while (divisor < dividand) 
	{
		i++;
		dividand -= divisor;
	}
	return i;
}
