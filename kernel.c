#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "shellmemory.h"
#include "shell.h"
#include "kernel.h"
#include "cpu.h"
#include "ram.h"
//#include "pcb.h"
extern FILE *ram[10];

//EACH CELL == A PAGE FRAME
PCB *head = NULL; //ready queue pointers
PCB *tail = NULL; //ready queue pointers
//The very first process is called root (Kernel), the very first thing that loads up into RAM, main.

void addReady(PCB *newPcb)
{
  newPcb->next = NULL;    /* clear next field in pcb to insert */
  if (head == NULL) //if ready queue is NULL
  {
    head = newPcb; // head points to the newly created PCB
  }
  //if NOT empty
  else
  {
    tail->next = newPcb;
  }
  tail = newPcb; //last inserted pcb as tail
}

// search the pcb that has the given frame, returns the pcb and sets npage to the
// page that is loaded at the given frame
PCB *searchFramePCB(int frame, int *nPage) {
    int found;
    PCB *pcb;
    int i;
    
    pcb = head;
    while (pcb!=NULL) {
        found = 0;
        for (i=0; i<pcb->pages_max; i++)
            if (pcb->pageTable[i] == frame) {
                found = 1;
                break;
            }
        if (found) {
            *nPage = i;
            return pcb;
        }
        pcb = pcb->next;
    }
    return NULL;
}


int pageFault(PCB *pcb)
{
    pcb->PC_page++;
    if (pcb->PC_page >= pcb->pages_max)
        return 1;   // program terminated
    if (pcb->pageTable[pcb->PC_page] != -1)
    {
        pcb->PC = ram[pcb->pageTable[pcb->PC_page]];  // get frame from ram
        pcb->PC_offset = 0;
    } 
    else
    {
        FILE *page = findPage(pcb->PC_page, pcb->PC);
        int frame = findFrame(page);
        int victimFrame, nVictimPage;
        PCB *victimPCB;
        if (frame == -1) { // no frame available
            victimFrame = findVictim(pcb);
            victimPCB = searchFramePCB(victimFrame, &nVictimPage);
        }
        updateFrame(frame, victimFrame, page);
        updatePageTable(pcb, pcb->PC_page, frame, victimFrame);
        if (frame == -1) {
            updatePageTable(victimPCB, nVictimPage, -1, -1);
        }
        pcb->PC = ram[pcb->pageTable[pcb->PC_page]];
        pcb->PC_offset = 0;
    }
   
    return 0;
}


int scheduler()
{
  //REMINDER: CPU EXECUTES PROGRAM
  //TA: use free
  PCB* current;
  int val = 0;

  if (head == NULL)
  {
      printf("There is nothing to schedule\n");
      exit;
  }

  while (head != NULL) //while there are programs waiting in the readyqueue
  {
   
    current = head; //our current PCB pointer gets the first pcb of the readyQueue
    cpu.IP = current -> PC; //It's time for the CPU to execute the file
    cpu.offset = current->PC_offset;
    val = run(cpu.quanta);
    head = head->next;

      if (val == 0) //if the file is not yet finished and quanta had run out
      {
        current->PC = cpu.IP; //the current pcb is updated
        current->PC_offset = cpu.offset;
        addReady(current); //add back into end of the queue
      } else if (val == 1) // program terminated
      {
        for(int i=0; i<current->pages_max; i++)
            if(current->pageTable[i] != -1)
                removeFile(ram[current->pageTable[i]]);
        free(current);
      } else  // page fault
      {
        if (pageFault(current)) 
        {
            for(int i=0; i<current->pages_max; i++)
                if(current->pageTable[i] != -1)
                    removeFile(ram[current->pageTable[i]]);
            free(current);
        }
        else
        {
            addReady(current); //add back into end of the queue
        }
      }
  }
}

void boot() {
    //remove everything from ram
    for (int i = 0; i < 10; i++)
    {
        ram[i] == NULL; //make each column == NULL
    }
    //backing store

    system("rm -fr ./backingstore"); //or else, remove the directory
    system("mkdir ./backingstore"); //create backingstore
}


int main(void)
{
   boot();
  //second action
	char buffer[1000];
	int finish = 0;
    int result = 0;

	initShellMemory(); //clean/initialize shellMem

	printf("Welcome to Owen's shell!\n");
	printf("Version 1.0 Created January 2019\n");
  while (!finish)
  {

    if(fgets(buffer,1000, stdin)!=NULL)
    {
    			result = prompt(buffer);
    			   if (result == 99)
             {
               finish = 1;
             }
    		     }
    		       else
               {
                 finish = 1;
               }// if there's no more input, exit

  }
    releaseShellMemory();
	printf("Goodbye! Thanks for using the Hansheng shell.\n");
	return 0;
}
