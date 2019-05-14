#include <stdio.h>
#include "interpreter.h"
#include "shell.h"
#include "cpu.h"

struct CPU cpu = {NULL, "", 2, 0}; 

int run(int quanta)
{
	char *args[100];
	int result = 0;

    for(int i = 0; i < quanta; i++)
    {
        if(fgets(cpu.IR, 1000, cpu.IP)!= NULL)
        {
            parse(cpu.IR,args);
            result = interpreter(args);
            if (result == 99)
            {
                result = 1; //end program once user enters "quit"
                break;
            }
            cpu.offset++;
            if (cpu.offset == 4) {
                cpu.offset = 0;
                return -1; // page fault
            }
        }
        else //if reach end of file of instruction pointer
        {
            result = 1;	// program ended, result == 1
            break;
        }
    }
    return result;
}
