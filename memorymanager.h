//Public functions
int countTotalPages(FILE *f);
FILE* findPage(int pagenumber, FILE* file);
int findFrame(FILE *page);
int findVictim(PCB *p);
int updateFrame(int frameNumber, int victimFrame, FILE *page);
int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame);
int launcher(FILE* p);

PCB *getVictimPCB(int victimFrame, int *nPage);