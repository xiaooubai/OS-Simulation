#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "shellmemory.h"
#include "shell.h"
#include "kernel.h"

int run_command(char *filename) {
	FILE *ptr;
	char buffer[1000];
	int result = 0;

	ptr = fopen(filename,"rt");
	if (ptr == NULL) {
		printf("Script filename not found\n");
		return 4; // file not found
	}

	fgets(buffer,999,ptr);
	while(!feof(ptr)) {
		if (strlen(buffer)>1) result = prompt(buffer);
		if (result == 99) break;
		fgets(buffer,999,ptr);
	}

	fclose(ptr);

	if (result == 99) return 99;

	return 0;
}

int execute(char *program[])
{
	int i = 1;
	FILE *fp;

	//check to make sure thre arent same files
	while (program[i] !=NULL)
	{
		fp = fopen(program[i], "rt");
		if (fp == NULL)
		{
			printf("Unable to open script: \"%s\"\n", program[i]); // error opening script
		}

		else
		{
		    launcher(fp); //load to memory
		}
		i++;
	}
	scheduler();	// begin scheduling
	return 0;
}




int interpreter(char *args[])
{
	int result = 0; // no errors
    char *var;

	if (args[0] == NULL || args[0][0]=='\0')	// no command entered
		return result;

	if (strcmp(args[0],"help")==0)
	{
		printf("Legal commands:\n");
		printf("help              display this help\n");
		printf("quit              exits the shell\n");
		printf("set VAR STRING    assign STRING to VAR\n");
		printf("print VAR         display contents of VAR\n");
		printf("run SCRIPT.TXT    interpret SCRIPT.TXT\n");
		printf("exec p1 p2 p3     executes concurrent programs\n");

		result = 0;
	}
	else if (strcmp(args[0],"quit")==0)
	{
		if (args[1] != NULL)
		{
			printf("Wrong number of quit parameters\n");
			return 1; // exit error
		}
		result = 99; // exit shell code
	}
	else if (strcmp(args[0],"set")==0)
	{
		if (args[1] == NULL || args[2] == NULL || args[3] != NULL)
		{
			printf("Wrong number of set parameters\n");
			return 1; // set error
		}

		add(args[1], args[2]);
	}
	else if (strcmp(args[0],"print")==0) {
		if (args[1] == NULL || args[2] != NULL) {
			printf("Wrong number of print parameters\n");
			return 2; // print error
		}
		var = get(args[1]);
        if (var != NULL)
    		printf("%s\n", var);
	}
	else if (strcmp(args[0],"run")==0)
	{
		if (args[1] == NULL)
		{
			printf("Run is missing filename\n");
			return 3; // run error
		}
		if (args[2] != NULL) {
			printf("Wrong number of run parameters\n");
			return 3; // run error
		}

		result = run_command(args[1]);
	}
	else if (strcmp(args[0],"exec")==0)
	{

			if (args[1] == NULL && args[2] == NULL && args[3] == NULL)
			{
				printf("There is nothing to execute..");
				result = 1;
			}
			else if (args[1] != NULL && args[2] == NULL && args[3] == NULL)
			{
				if (!strcmp(strrchr(args[1], '\0') - 4, ".txt"))
				{
						result = execute(args);
				} else {
					printf("this is not a .txt file");
					result = 1;
				}
			}
			else if (args[1] != NULL && args[2] != NULL && args[3]==NULL)
			{
				if (!strcmp(strrchr(args[1], '\0') - 4, ".txt") && strcmp(strrchr(args[2], '\0') - 4, ".txt"))
				{
					printf("one of your files is not a proper .txt file");
					result = 1;
				}
				if (strcmp(strrchr(args[1], '\0') - 4, ".txt") && !strcmp(strrchr(args[2], '\0') - 4, ".txt"))
				{
					printf("one of your files is not a proper .txt file");
					result = 1;
				}
				else
				{
					result = execute(args);
				}
			}
			else if (args[1] != NULL && args[2] != NULL && args[3]!=NULL)
			{
				if (!strcmp(strrchr(args[1], '\0') - 4, ".txt") && !strcmp(strrchr(args[2], '\0') - 4, ".txt") && strcmp(strrchr(args[3], '\0') - 4, ".txt")  )
				{
					printf("one of your files is not a proper .txt file");
					result = 1;
				}
				if (!strcmp(strrchr(args[1], '\0') - 4, ".txt") && strcmp(strrchr(args[2], '\0') - 4, ".txt") && strcmp(strrchr(args[3], '\0') - 4, ".txt"))
				{
					printf("one of your files is not a proper .txt file");
					result = 1;
				}
				if (strcmp(strrchr(args[1], '\0') - 4, ".txt") && !strcmp(strrchr(args[2], '\0') - 4, ".txt") && strcmp(strrchr(args[3], '\0') - 4, ".txt"))
				{
					printf("one of your files is not a proper .txt file");
					result = 1;
				}
				if (!strcmp(strrchr(args[1], '\0') - 4, ".txt") && strcmp(strrchr(args[2], '\0') - 4, ".txt") && !strcmp(strrchr(args[3], '\0') - 4, ".txt"))
				{
					printf("one of your files is not a proper .txt file");
					result = 1;
				}
				if (strcmp(strrchr(args[1], '\0') - 4, ".txt") && !strcmp(strrchr(args[2], '\0') - 4, ".txt") && !strcmp(strrchr(args[3], '\0') - 4, ".txt"))
				{
					printf("one of your files is not a proper .txt file");
					result = 1;
				}
				if (strcmp(strrchr(args[1], '\0') - 4, ".txt") && strcmp(strrchr(args[2], '\0') - 4, ".txt") && !strcmp(strrchr(args[3], '\0') - 4, ".txt"))
				{
					printf("one of your files is not a proper .txt file");
					result = 1;
				}
				else
				{
					result = execute(args);
				}

			}

			if (args[1] == NULL || (args[1] != NULL && args[2] != NULL && args[3] != NULL && args[4] != NULL))
			{
				printf("Wrong number of exec parameters\n");
				return 5;
			}

		// else if (!strcmp(strrchr(args[1], '\0') - 4, ".txt")
		// 				&&  args[2] != NULL && strcmp(strrchr(args[2], '\0') - 4, ".txt")
		// 				|| !strcmp(strrchr(args[1], '\0') - 4, ".txt")
		// 				&&  args[2] != NULL && !strcmp(strrchr(args[2], '\0') - 4, ".txt")
		// 				&& args[3] != NULL && !strcmp(strrchr(args[3], '\0') - 4, ".txt"))
		// 				{
		// printf("You have inputted an executable .txt file but also inputted something that is not.\n");
		// printf("please make sure all the files you input are valid!\n");
		// return 5;
		// }
	}



	else {
		printf("Command does not exist\n");
		result = 98; // command does not exist
	}
	return result;
}
