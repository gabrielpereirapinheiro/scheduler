/*
	Gabriel Pereira Pinheiro - gabriel.pereira.pinheiro@gmail.com
	Ismael Cordeiro - ismael@gmail.com
	University of Brasilia - 2018
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

/*
Function to analyse arguments from terminal
*/
argument processArguments (int value, char * arguments[])
{
	argument args ;
	
	//Define variable time 
	strcpy(args.time,arguments[1]);
	
	//Check if copies its a number bigger than 0
	if(atoi (arguments[2])<1)
	{	
		//If equals 0 or less, its a invalide number
		printf("\nIlegal number of copies\nEnd of execution\n");
		exit(0);
	}
	//Transform a string to int
	args.copies = atoi (arguments[2]);	

	if(value==4)
	{
		//Copy string to another string
		strcpy(args.name,arguments[3]);
	
		//Default priority
		args.priority = 1;				
	}
	else
	{
		args.priority = atoi (arguments[3]);
		strcpy(args.name,arguments[4]);

		if(args.priority<1 || args.priority>3)
		{
			printf("\nIlegal priority\nEnd of execution\n");
			exit(0);
		}
	}
	return args;
}

int main(int argc, char *argv[ ])
{	
	if(argc!=4 && argc!=5)
	{
		printf("\nMissing arguments\n");
		return 0;
	}
	
	argument args = processArguments(argc,argv);
	  
	printf("The name is: %s\n\n",args.name);
	
	printf("Time:  %s\n\n",args.time);

	printf("Priority: %d\n\n",args.priority);
	
	printf("Number of copies: %d\n\n",args.copies);	

	return 0;
}