#include <stdlib.h>
#include <stdio.h>
#include "interpreter.h"

int parse(char buffer[], char *args[])
{
	int i = 0;
	int j = 0;

	while (i<1000 && buffer[i]!='\0' && buffer[i]!='\n' && buffer[i]!='\r')
	{
			//while i is less than 1000
			// skip white space
			while(i<1000 && buffer[i]==' ') i++; //eliminates white spaces beforehand

			// extract current arg

			args[j++] = &buffer[i];
			while(i<1000 && buffer[i]!=' ' && buffer[i]!='\n' && buffer[i]!='\r')
			{
				i++;
			}
			buffer[i++] = '\0'; //get command
	}
	args[j++] = NULL;
	// debug code: printf("[%s] [%s] [%s]\n", arg0, arg1, arg2);
	// check if there is more data (illegal)
	if (i<1000 && (buffer[i]=='\n' || buffer[i]=='\r' || buffer[i]=='\0'))
		return 1; // completed successfully (true)
	else
		return 0;
}

int prompt(char buffer[]) {
	char *args[999];
	int result = 0;

	result = parse(buffer,args); //parse input

	if (!result)
	{
		printf("Error: Command %s has too many characters\n",buffer);
	}


	result = interpreter(args);

	return result;
}
