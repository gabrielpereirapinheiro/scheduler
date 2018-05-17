/**
 *	Gabriel Pereira Pinheiro - gabriel.pereira.pinheiro@gmail.com
 *	Ismael Coelho Medeiros - 140083162@aluno.unb.br
 *	University of Brasilia - 2018
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct time
{
	int hours;
	int minutes;
};
typedef struct time Time;

struct process
{
	Time time;
	int copies;
	int priority;
	char name[64];
};
typedef struct process Process;

/**
 * Function to process arguments received from terminal.
 */
int processArguments(int nArgs, char *args[], Process* process);

int main(int argc, char *argv[])
{
	Process args;
	if (processArguments(argc, argv, &args) == -1) {
		printf("Something went wrong!! This program will be closed.\n");
		return -1;
	}

	printf("The name is: %s\n\n", args.name);

	printf("Time:  %.2d:%d\n\n", args.time.hours, args.time.minutes);

	printf("Priority: %d\n\n", args.priority);

	printf("Number of copies: %d\n\n", args.copies);

	return 0;
}

int processArguments(int nArgs, char *args[], Process* process)
{
	if (nArgs != 4 && nArgs != 5)
	{
		printf("ARGUMENT ERROR: Missing arguments\n");
		return -1;
	}

	// Define 1st argument: time
	char timeStrLength = strlen(args[1]);
	if (timeStrLength != 4 && timeStrLength != 5)
	{
		printf("ARGUMENT ERROR: Failed while parsing time string\n");
		return -1;
	}
	// Treat time string in the following format:
	// - h:MM
	// - hh:MM
	if (timeStrLength != 5)
	{
		char hoursStr[2];
		hoursStr[0] = args[1][0];
		hoursStr[1] = '\0';
		process->time.hours = atoi(hoursStr);
		char minutesStr[3];
		strncpy(minutesStr, &args[1][2], 2);
		minutesStr[2] = '\0';
		process->time.minutes = atoi(minutesStr);
	}
	else
	{
		char hoursStr[3];
		strncpy(hoursStr, args[1], 2);
		hoursStr[2] = '\0';
		process->time.hours = atoi(hoursStr);
		char minutesStr[3];
		strncpy(minutesStr, &args[1][3], 2);
		minutesStr[2] = '\0';
		process->time.minutes = atoi(minutesStr);
	}

	// Check if "copies" argument is bigger than zero.
	int copies = atoi(args[2]);
	if (copies < 1)
	{
		printf("ARGUMENT ERROR: Ilegal number of copies\n");
		return -1;
	}
	process->copies = copies;

	// Verify if priority was provided (nArgs = 5). If not,
	// set a default value.
	if (nArgs == 4)
	{
		// Copy string to another string. The maximum size of this
		// parameter is 63 characters.
		strncpy(process->name, args[3], 63);

		// Default priority.
		process->priority = 1;
	}
	else
	{
		// Store infomed priority.
		process->priority = atoi(args[3]);
		if (process->priority < 1 || process->priority > 3)
		{
			printf("ARGUMENT ERROR: Ilegal priority\n");
			return -1;
		}

		// Copy string to another string. The maximum size of this
		// parameter is 63 characters.
		strncpy(process->name, args[4], 63);
	}

	return 0;
}