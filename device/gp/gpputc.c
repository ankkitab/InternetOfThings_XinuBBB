#include <xinu.h>

devcall gpputc(struct dentry *devptr, char val)
{
	unsigned int pin = 0;

	switch (devptr->dvminor)
	{
		case 0:
			pin = PIN_0_NUM;
		break;

		case 2:
			pin = PIN_2_NUM;
		break;
	}
	
	
	//code to write stuff usinfg the registers
	if(val == 0)
	{
		printf("gpputc: clear bit %u %u,\n", pin, devptr->dvcsr + GPIO_CLEARDATAOUT);
		*((volatile unsigned int*)(devptr->dvcsr + GPIO_CLEARDATAOUT)) = 1<<pin;
	}
	else if (val == 1)
	{
		printf("gpputc: set bit %u %u,\n",pin, devptr->dvcsr + GPIO_SETDATAOUT);
		*((volatile unsigned int*)(devptr->dvcsr + GPIO_SETDATAOUT)) = 1<<pin;
	}

	return OK;
}