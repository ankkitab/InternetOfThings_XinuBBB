#include <xinu.h>

devcall gpinit(struct dentry *devptr)
{
	// unsigned int ipPin;

	// if(PIN1_MODE == 0)
	// 		ipPin = PIN1;

	// else if(PIN2_MODE == 0)
	// 		ipPin = PIN2;

	//*((volatile unsigned int*)(devptr->dvcsr + GPIO_IRQSTATUS_SET_0)) = 1 << PIN_IN;
	//*((volatile unsigned int*)(devptr->dvcsr + GPIO_RISINGDETECT)) = 1 << PIN_IN;
	//*((volatile unsigned int*)(devptr->dvcsr + GPIO_FALLINGDETECT)) = 1 << PIN_IN;


	//configuring the input pin
	// unsigned int opPin;
	// if(PIN1_MODE == 1)
	// 	opPin = PIN1;
	// else if(PIN2_MODE == 1)
	// 	opPin = PIN2;

	kprintf("gpinit 1\n");

	*((volatile unsigned int*)(devptr->dvcsr + GPIO_OE)) = ~(1<<PIN_0_NUM);

	kprintf("gpinit 2\n");

	return OK;
}
