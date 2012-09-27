/* EECS 370 LC-2K Instruction-level simulator */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

struct int25 {
	unsigned int data : 25;
	int signedData;
};

struct int16 {
	unsigned int data : 16;
};

struct int3 {
	unsigned int data : 3;
};

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
void printMem(stateType *);
int convertNum(int);

int
main(int argc, char *argv[])
{
		int numLines = 0;
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
	printf("error: usage: %s <machine-code file>\n", argv[0]);
	exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
	printf("error: can't open file %s", argv[1]);
	perror("fopen");
	exit(1);
    }

    /* read in the entire machine-code file into memory */
		for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
			state.numMemory++, numLines++) {
	
			struct int25 numMem25;
		
			if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
			    printf("error in reading address %d\n", state.numMemory);
			    exit(1);
			}
			// printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);

			// SP: Force 25-bit two's complement integer
			int num = *(state.mem+state.numMemory);
	
			// The opcode is an add 1 0 0 (524288) or higher
			if (num >= 524288) {
				numMem25.data = num;
			}
			// Otherwise it's a .fill
			else {
				numMem25.signedData = num;
			}
  	}

		printMem(&state);
		

    return(0);
}

void
printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
	for (i=0; i<statePtr->numMemory; i++) {
	    printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}
    printf("\tregisters:\n");
	for (i=0; i<NUMREGS; i++) {
	    printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}
    printf("end state\n");
}

void
printMem(stateType *statePtr)
{
	int i;
	for (i=0; i<statePtr->numMemory; i++) {
			printf("memory[%d]=%d\n", i, statePtr->mem[i]);
	}
}

  int
  convertNum(int num)
  {
/* convert a 16-bit number into a 32-bit Linux integer */
if (num & (1<<15) ) {
    num -= (1<<16);
}
return(num);
  }