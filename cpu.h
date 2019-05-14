// Definition of the CPU structure
struct CPU {
	FILE *IP;
	char IR[1000];
	int quanta;
	int offset;
};

extern struct CPU cpu; //created global var because there is just one cpu

int run(int quanta);
