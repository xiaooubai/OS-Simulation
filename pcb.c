#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include<pthread.h>
#include "pcb.h"

PCB* createPCB(FILE *p)
{
  PCB* pcb;
  pcb = (PCB*)malloc(sizeof(PCB));
  pcb -> PC = p; //pcb.PC = input p
  pcb->next =NULL;
  return pcb;
}
