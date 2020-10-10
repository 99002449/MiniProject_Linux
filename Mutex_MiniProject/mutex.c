#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/msg.h>
pthread_t t1,t2; 
int counter; 
pthread_mutex_t mu; 
const int number=6;

struct message 
{
    long msgtype; 	
    int banking[number];
}service; 



void receive()
{
	key_t key;
	int msgid;
	int ret;
	int i; 
	key = ftok("progfile", 65);
	msgid = msgget(key, 0666 | IPC_CREAT); 
	msgrcv(msgid, &service, sizeof(service), 1, 0);
        for(i = 0; i< number; i++)
 		printf("Customer %d type of service is: %d \n",(i+1), service.banking[i]);
 	return ;
}


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
    	printf("Welcome to the Beauty SPA center\n");
	for(i=0;i<number;i++)
	{
  		printf("\nCustomer %d -Enter the type of service\n",i+1);
  		printf("Enter 0 -> Deposit/Withdaw  1 -> Query: ");
 		scanf("%d",&service.banking[i]);
	}
    	ret = msgsnd(msgid, &service, sizeof(service), 0); 
    	if(ret < 0)
    	{
	    perror("Message queue not send");
	    exit(2);
   	}
  	return ; 
}  
void* Deposit_Withdraw(void* arg) 
{ 
	pthread_mutex_lock(&mu); 
	int  i = 0; 
	int DepositWithdraw=0;
	printf("Thread1 started\n"); 
    	for(i = 0; i <number; i++)
	{
		if(service.banking[i]==0) 
			DepositWithdraw=DepositWithdraw+1;
	}
	printf("Number of customers for Deposit/Withdraw are:%d\n",DepositWithdraw);
   	printf("Thread1 finished\n"); 
  	pthread_mutex_unlock(&mu); 
  	return NULL; 
} 

void* Query(void* arg) 
{ 
	pthread_mutex_lock(&mu); 
	int  i = 0; 
	int quer=0;
   	printf("Thread2 started\n"); 
   	for(i = 0; i < number; i++)
	{
		if(service.banking[i]==1) 
		quer=quer+1;
	}
	
   	printf("Number of customers for Query are:%d\n",quer);
    	printf("Thread2 finished\n"); 
  	pthread_mutex_unlock(&mu); 
  	return NULL; 
} 
  
int main(void) 
{ 
 	send();
    	receive();
    	int i = 0; 
    	int error; 
  
    	if (pthread_mutex_init(&mu, NULL) != 0) 
    	{ 
        	printf("\n Mutex init has failed\n"); 
       	 return 1; 
    	} 
    	else
    	{
		printf("You are in mutex\n");
    	}
    	error = pthread_create(&t1,NULL,&Deposit_Withdraw, NULL); 
    	if (error != 0) 
           printf("\nThread can't be created :[%s]", strerror(error)); 
   	 error = pthread_create(&t2,NULL,&Query, NULL); 
    	if (error != 0) 
           printf("\nThread can't be created :[%s]", strerror(error));   
   	pthread_join(t1, NULL); 
    	pthread_join(t2, NULL); 
    	pthread_mutex_destroy(&mu);
    	return 0; 
} 
