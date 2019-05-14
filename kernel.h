#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "pcb.h"
#include "memorymanager.h"

//typedef struct node
//{
//  PCB *pcb;
//  struct node *next;
//} linkedlistNode;


void create(FILE *p);
void addReady(PCB *newPcb);
int scheduler();

PCB *searchFramePCB(int frame, int *nPage);