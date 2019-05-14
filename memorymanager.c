#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include "shellmemory.h"
#include "shell.h"
#include "kernel.h"
#include "cpu.h"
#include "ram.h"

int nBackingFiles = 0;

int countTotalPages(FILE *f) {
    char line[BUFSIZ];
    int nlines;
    
    nlines = 0;
    while(fgets(line, BUFSIZ, f) != NULL)
        nlines++;
    
    return (nlines - 1)/4 + 1;
}

FILE* findPage(int pagenumber, FILE* file) {
    FILE *fp2;
    int page;
    char buffer[BUFSIZ];

    fp2 = fdopen (dup (fileno (file)), "r");
    fseek(fp2, 0, SEEK_SET);
   
    for (page = 0; page < pagenumber; page++) {
        fgets(buffer, BUFSIZ, fp2);
        fgets(buffer, BUFSIZ, fp2);
        fgets(buffer, BUFSIZ, fp2);
        fgets(buffer, BUFSIZ, fp2);
    }
    return fp2;
}

int findFrame(FILE *page) {
/*Use the FIFO technique to search ram[] for a frame (not equal to NULL). If one
exists then return its index number, otherwise return -1.*/
    for (int i = 0; i < 10; i++) {
        if (ram[i] == NULL) {
            return i;//index number
        }
    }
    return -1;
}

int findVictim(PCB *p) {
  /*This function is only invoke when findFrame() returns a -1. Use a random
number generator to pick a frame number. If the frame number does not belong to
the pages of the PCB (page table) then return that frame number, otherwise,
starting from the randomly selected frame, iteratively increment the frame number
(modulo-wise) until you come to a frame number not belong to the PCB’s pages,
and return that number.*/
    int victimFrame = rand() % 10; //random number
    int i;
    int found;
    
    found = 0;
    for (i=0; i<p->pages_max; i++)
        if (p->pageTable[i] == victimFrame) {
            found = 1;
            break;
        }
    if (!found) //if frame number does not belong to pages of the PCB
    {
        return victimFrame; //return the frame number
    } else { //otherwise
        while (found) {
            victimFrame++; //increment until it is not, and then return
            victimFrame %= 10;
            for (i=0; i<p->pages_max; i++)
                if (p->pageTable[i] == victimFrame) {
                    found = 1;
                    break;
                }
        }
        return victimFrame; //return vFrame
    }
}

int updateFrame(int frameNumber, int victimFrame, FILE *page) {
  /*Once we have a pointer to the page and the frame number (or victimFrame) then
we can put the page into memory. Since we are not handling dirty pages, then this
is easy. If the frameNumber is -1 then we use the victimFrame as the frame
number: Overwrite ram[frameNumber] or ram[victimFrame] with FILE
*page.*/
    if (frameNumber == -1) { //if fN = -1, we use victimFrame as number
        ram[victimFrame] = page;
        return victimFrame;
    } else {
        ram[frameNumber] = page;
    }
    return 0;
}


int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame) {
  /*The PCB’s page table must also be updated to reflect the changes. If a victim was
selected then the PCB page table of the victim must also be updated. We do this
once for the PCB asking for the page fault, and we might do it again for the victim
PCB (if there was one).
p->pageTable[pageNumber] = frameNumber (or = victimFrame).*/
    if (frameNumber == -1) {
        p->pageTable[pageNumber] = victimFrame;
    } else {
        p->pageTable[pageNumber] = frameNumber;
    }
    return 0;
}

/*Modify the PCB by adding an array: int pageTable[10]; The index of the array is the
page number. The values stored in the cell is the frame number. The array is size 10
because RAM is size 10 in our simulator. The PC must be the offset from the beginning of a
frame, where offset is the line count starting from zero. Keep FILE *PC as the pointer to
the current position in the file. Add int PC_page, PC_offset, pages_max. This
tracks which page and offset the program is currently at, and the total number of pages in
this program.*/


int launcher(FILE* p) {
    char ch;
    char filename[50];
    FILE *newFile;
    PCB *pcb, *victimPCB;
    int nPage;
    FILE *page;
    int frame, victimFrame, nVictimPage;
    
    sprintf(filename, "./backingstore/program%d", nBackingFiles++);
    newFile = fopen(filename, "w+t");
    
    while((ch = fgetc(p)) != EOF)
        fputc(ch, newFile);
    fclose(p);
    fseek(newFile, 0, SEEK_SET);
 
    pcb = createPCB(NULL); //create a PCB
    pcb->PC_page = 0;
    pcb->PC_offset = 0;
    pcb->pages_max = countTotalPages(newFile);
    for (nPage=0; nPage < pcb->pages_max; nPage++)
       pcb->pageTable[nPage] = -1;  // all pages have no frames

    for (nPage=0; nPage < 2 && nPage < pcb->pages_max; nPage++)  { // load 2 pages
        page = findPage(nPage, newFile);

        frame = findFrame(page);
        if (frame == -1) { // no frame available
            victimFrame = findVictim(pcb);
            victimPCB = searchFramePCB(victimFrame, &nVictimPage);
        }
        updateFrame(frame, victimFrame, page);
        updatePageTable(pcb, nPage, frame, victimFrame);
        if (frame == -1) {
            updatePageTable(victimPCB, nVictimPage, -1, -1);
        }
    }

    pcb->PC = ram[pcb->pageTable[0]];

    fseek(newFile, 0, SEEK_SET);

    addReady(pcb);
}
//Launching a program consists of the following steps:
/*Copy the entire file into the backing store.
2. Close the file pointer pointing to the original file.
3. Open the file in the backing store.
4. Our launch paging technique defaults to loading two pages of the program into RAM when it
is first launched. A page is 4 lines of code. If the program has 4 or less lines of code,
then only one page is loaded. If the program has more than 8 lines of code,
then only the first two pages are loaded. To do this,
implement the following helper functions that exist in the memorymanager.c file:*/
