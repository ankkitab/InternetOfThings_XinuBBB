#include<xinu.h>

devcall anlinit(struct dentry *devptr)
{
	*((volatile int*)(CM_WKUP_ADC_TSC_CLKCTRL)) = 0x02;	
	while(*((volatile int*)(CM_WKUP_ADC_TSC_CLKCTRL)) != 0x02);
	
	*((volatile int*)(devptr->dvcsr+CTRL)) = (0x01)<<2;

	*((volatile int*)(devptr->dvcsr+STEPCONFIG)) = ((0x00)<<19 | 0x100);
	*((volatile int*)(devptr->dvcsr+STEPDELAY)) = (0x0F)<<24;

	*((volatile int*)(devptr->dvcsr+CTRL)) = 0x01;

	*((volatile int*)(devptr->dvcsr+STEPENABLE)) = (0x1)<<1;
}