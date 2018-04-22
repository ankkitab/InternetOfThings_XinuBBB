#include<xinu.h>


devcall anlread(
				struct dentry *devptr,
				char* buf,
				int32 count)
{	int i;

	kprintf("Inside anl read1");
	while(!(*((volatile int*)(devptr->dvcsr+FIFOCOUNT)) & 0x3F) && i<10)
	{
		kprintf("FIFOCOUNT reg %u, %u\n",(volatile int*)(devptr->dvcsr+FIFOCOUNT),*((volatile int*)(devptr->dvcsr+FIFOCOUNT)) & 0x3F);
		sleep(1);
		i++;
	}

	kprintf("Inside anl read2");

	int val = (*((volatile int*)(devptr->dvcsr+FIFODATA)) & 0x3FF);
	kprintf("Inside anl read2 val:%d", val);
  	sprintf(buf, "%d", val);	
  	kprintf("char value: %s", buf);
  	//sprintf(buf, "%d", (*((volatile int*)(FIFO1DATA)) & 0xFFF));	
}