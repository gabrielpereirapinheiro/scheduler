#include <stdio.h>

int processArguments (int value)
{
	int condition = 1;

	if(value!=4)
	{
		condition = 0;
	}	
	return condition;
}


int main( int argc, char *argv[ ] )
{
	
	int cont;
	
	int valideArguments = processArguments(argc);

	if(valideArguments==0)
	{
		printf("\nMissing arguments\n");
		return 0;
	}	
    
	for(cont=0; cont < argc; cont++)
        printf("%d Parametro: %s\n", cont,argv[cont]);
    return 0;
}