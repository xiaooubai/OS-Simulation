//This is the PCB
typedef struct pcb_s
{
  FILE *PC;
  int pageTable[10];
  int PC_page;
  int PC_offset;
  int pages_max;

  struct pcb_s *next;
}PCB;
//create PCB
PCB* createPCB(FILE *p);
