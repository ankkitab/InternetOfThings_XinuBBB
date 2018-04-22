#include <xinu.h>

devcall gpgetc(struct dentry *devptr)
{

	if(devptr->dvminor == 1)
	{
		volatile unsigned  int* dataIn = devptr->dvcsr + GPIO_DATAIN;
		
		unsigned int mask = 0;
		mask = 1<<PIN_1_NUM;
		mask = *(dataIn)&mask;
		mask = mask>>PIN_1_NUM;
		return (devcall)mask;
	}
	// printf("gpgetc: %u, %d\n", devptr->dvcsr + GPIO_DATAIN, val);
	// //code to write stuff usinfg the register

	return (devcall)-1;
}