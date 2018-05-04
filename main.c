/*
Gabriel Pereira Pinheiro
Ismael 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Argument
{
	char time[5];
	int copies;
	int priority;
	char name[15];
}argument;

argument processArguments (int value, char * arguments[])
{
	argument args ;
	strcpy(args.time,arguments[1]);
	args.copies = atoi (arguments[2]);	
	if(value==4)
	{
		strcpy(args.name,arguments[3]);				
	}
	else
	{
		args.priority = atoi (arguments[3]);
		strcpy(args.name,arguments[4]);
	}
	printf("Nome %s\n\n",args.name);

	printf("tempo %s\n\n",args.time);

	printf("priorirdade %d\n\n",args.priority);
	
	printf("copias %d\n\n",args.copies);	
}

int main( int argc, char *argv[ ] )
{	
	if(argc!=4 && argc!=5)
	{
		printf("\nMissing arguments\n");
		return 0;
	}
	
	processArguments(argc,argv);
	  
	return 0;
}