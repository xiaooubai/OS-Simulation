#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include<pthread.h>

FILE *ram[10];

void removeFile(FILE *file)
{
  for (int i = 0; i < 10; i++)
  {
    if (ram[i] == file)
    {
      fclose(ram[i]);
      ram[i] = NULL;
      break;
    }
  }
}
