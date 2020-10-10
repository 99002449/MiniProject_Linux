#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

const int number=6;
struct message {
    long msgtype; 	
    int banking[number]; 
} service; 
  
void send() 
{ 
	key_t key; 
	int msgid; 
    	int ret,i;
    	key = ftok("progfile", 65); 
    	msgid = msgget(key, 0666 | IPC_CREAT);
    	if(msgid < 0) 
    	{
		perror("Error in creating msgid");
		exit(1);
    	}
    	service.msgtype = 1; 
    	printf("Welcome to the Bank\n");
    	for(i=0;i<number;i++)
	{
		printf("\nCustomer %d -Enter the type of service\n",i+1);
  		printf("Enter 0 -> Deposit/Withdaw  1 -> Query: ");
 		scanf("%d",&service.banking[i]);
	}
    	ret = msgsnd(msgid, &service, sizeof(service), 0); 
    	if(ret < 0) 
    	{
	    perror("message queue not send");
	    exit(2);
   	}
    	return; 
} 
int val=0;

void* filled_slot(void *arg)
{
	int i,sum1=0;
	printf("\nThread providing the service to Customers \n");
	while(val<number)
  	{
   		/*Wait for read thread to print*/
    
  		sem_wait((sem_t *) arg);
   		val++;
	}
	
}

void* car_service(void *arg)
{
	while(val<number)
  	{
		if(service.banking[val]==1)
		{
    			printf("Customer %d Deposit/Withdraw is done\n",(val+1));
		}
		else
 			printf("Customer %d Query solving is done\n",(val+1));
    		
    		sem_post((sem_t *) arg);
       	sleep(1);  
  	}
}


void recv()
{
    	key_t key;
        int msgid;
     	int ret;
     	int i; 
    	key = ftok("progfile", 65);
    	msgid = msgget(key, 0666 | IPC_CREAT); 
    	msgrcv(msgid, &service, sizeof(service), 1, 0);
    	printf("\n");
        for(i = 0; i< number; i++)
        {
 		printf("Customer %d type of service is: %d--->",(i+1), service.banking[i]);
 		if( service.banking[i]==1)
 			printf("(Query solving)\n");
 		else
 			printf("(Deposit/Withdraw)\n");
        }
 	return ;
}

int main()
{
	send();
	recv();
	sem_t rs;
	void* res;
	pthread_t pt2,pt1;
	sem_init(&rs,0,0);
	pthread_create(&pt1,NULL,filled_slot,&rs);
	pthread_create(&pt2,NULL,car_service,&rs);
	pthread_join(pt1,&res);
	pthread_join(pt2,&res);
	sem_destroy(&rs);
}



