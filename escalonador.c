#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#define quantityOfProcess 10


typedef struct Process
{
	char time[5];
	int copies;
	int priority;
	char name[15];
}process;



int main(int argc, char *argv[ ])
{
	process processReady[quantityOfProcess];
	process processWainting[quantityOfProcess];



	return 0;
}