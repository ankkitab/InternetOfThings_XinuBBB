#include<xinu.h>
#define MAX_QUEUE_COUNT 10

typedef struct strCircularQueue
{
    int     front;
    int     rear;
    int     itemCount;
    uint32     data[MAX_QUEUE_COUNT];
} circularQueue;

circularQueue queue;

void initializeQueue(circularQueue *queue)
{
    int i;
    queue->itemCount   =  0;
    queue->front       =  0;
    queue->rear        =  0;

    for(i=0; i<MAX_QUEUE_COUNT; i++)
    {
        queue->data[i] = 0;
    }        
    return;
}

int isEmpty(circularQueue *queue)
{
    if(queue->itemCount)
        return 0;
    else
        return 1;
}

 int putItem(circularQueue *queue, uint32 value)
{
    if(queue->itemCount>=MAX_QUEUE_COUNT) //Overwrite condition
    {
        queue->data[queue->rear] = value;
        queue->rear = (queue->rear+1)%MAX_QUEUE_COUNT;
        return 0;
    }
    else
    {
        queue->itemCount++;
        queue->data[queue->rear] = value;
        queue->rear = (queue->rear+1)%MAX_QUEUE_COUNT;
        return 1;
    }
}

int getItem(circularQueue *queue, uint32 *value)
{
    if(isEmpty(queue))
    {
        return 0;
    }
    else
    {
        *value=queue->data[queue->front];
        queue->front=(queue->front+1)%MAX_QUEUE_COUNT;
        queue->itemCount--;
        return 1;
    }
}

void printQueueStats(circularQueue* queue)
{
    kprintf("front : %d, rear: %d \n",queue->front, queue->rear);
}

/*
*******************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************
START OF PROJECT CODE
*******************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************
*/

circularQueue theQueue;
sid32 mtxQueue;

pid32 producer_id;
pid32 consumer_id;

int32 rcv_UDP_data(char * output) {
	uint32 my_ip = getlocalip();
	uint32 my_receiver_port = 5954;
	uint32 pc_ip = PC_IP;
	uint32 pc_sender_port = 7599;
	char temp[8];

	kprintf("Beagle bone IP: %u\n", my_ip);
	kprintf("Beagle bone reciever port: %d\n", my_receiver_port);
	kprintf("PC IP: %u\n", pc_ip);
	kprintf("PC sender port: %d\n", pc_sender_port);
	
   	uid32 slot = udp_register(pc_ip, pc_sender_port, my_receiver_port);
	
	int32 length = udp_recv(slot, temp, 8, 10000);
	kprintf("\nMessage received  %s with length %d\n", temp, length);

	memcpy(output, temp, length);
	
	udp_release(slot);

	return length;
}

void send_UDP_data(char *data) 
{
	uint32 my_ip = getlocalip();
	uint32 my_sender_port = 5955;
	uint32 pc_ip = PC_IP;
	uint32 pc_listener_port = 7598;

	kprintf("Beagle bone IP: %u\n", my_ip);
	kprintf("Beagle bone sender port: %d\n", my_sender_port);
	kprintf("PC IP: %u\n", pc_ip);
	kprintf("PC listener port: %d\n", pc_listener_port);
	kprintf("Data to send: %s\n", data);
	
   	uid32 slot = udp_register(pc_ip, pc_listener_port, my_sender_port);
	
	udp_send(slot, data, strlen((char *) data));

	udp_release(slot);
}


process fillbuffer()
{
	kprintf("Fill buffer started\n");
	while(1) 
	{		
		char dataFromPC[8];
		if (rcv_UDP_data(dataFromPC) > 0)
		{
			uint32 data = atoi(dataFromPC);

			wait(mtxQueue);
			kprintf("Adding %d\n", data);
			printQueueStats(&queue);
			putItem(&queue, data);
			signal(mtxQueue);
		}
        //sleep(1);

	}
}

process readfrombuffer()
{
	kprintf("Read buffer started\n");
	while(1)
	{
		uint32 data;

		wait(mtxQueue);
		if(getItem(&queue, &data))
		{
			kprintf("Processing %d\n", data);
			printQueueStats(&queue);

			if (data == 0) {
				putc(LED0, 0);
			} else if (data == 1) {
				putc(LED0, 1);
			} else if(data == 2) {
				//Do photo logic
				//  
				char str[33];
				read(TEMP0,str,33);

				kprintf("Sending data to UDP: %s\n", str);
				send_UDP_data(str);
			}
		}
		signal(mtxQueue);

        //sleep(1);
	}

	//putc(LED0, data);
}


process	main(void)
{
	recvclr();
	
	 kprintf("Inside process main\n");

	 mtxQueue = semcreate(1);

	 consumer_id = create(readfrombuffer, 4096, 50, "readfrombuffer",0);
     producer_id = create(fillbuffer, 4096, 50, "fillbuffer",0);	


	// //kprintf("%d\n", PIN_0_NUM); //test config
	// //send_UDP_data(data);

	// char dataFromPC[8];
	// rcv_UDP_data(dataFromPC);

	// kprintf("Message recieved: %s\n", dataFromPC);
	// uint32 pc_input = atoi(dataFromPC);


	// kprintf("%d",pc_input);

    resched_cntl(DEFER_START);
	
	resume(producer_id);
    resume(consumer_id);

    resched_cntl(DEFER_STOP);


	// uint32 val = 1;
	// while(1)
	// {
	// 	putc(LED0, val);
	// 	val = 1-val;
	// 	sleep(1);
	// }

	return OK;
}
