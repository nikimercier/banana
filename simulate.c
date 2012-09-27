/* EECS 370 LC-2K Instruction-level simulator */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

enum opcodes { 
	add,
	nand,
	lw,
	sw,
	beq,
	jalr,
	halt,
	noop
};

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

void evalState(stateType *);
struct int25 getInstruction(int);
void evalInstruction(struct int25, stateType *)
void printState(stateType *);
void printMem(stateType *);
int convertNum(int);

int
main(int argc, char *argv[])
{
	char line[MAXLINELENGTH];
	stateType state;
	FILE *filePtr;
	int i;

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
		state.numMemory++) {
	
		if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
		    printf("error in reading address %d\n", state.numMemory);
		    exit(1);
		}
	}

	// Show memory
	printMem(&state);
	
	// Zero all registers
	for (i = 0; i < NUMREGS; i++) {
		state.reg[i] = 0;
	}
	
	// Evaluate and print each state
	for (i = 0; i < state.numMemory; i++) {
		evalState(&state);
		printState(&state);
	}

	return(0);
}

struct int25 getInstruction(int decimal) {
	struct int25 instr;
	instr.data = 0;
	
	// The opcode is an add 1 0 0 (524288) or higher
	if (decimal >= 524288) {
		instr.data = decimal;
	}
	// Otherwise it's a .fill
	else {
		instr.signedData = decimal;
	}
	
	return instr;
}

void evalInstruction(struct int25 instr, stateType *statePtr) {
	
}

// Evaluate given state.
void evalState(stateType *statePtr) {
	int pc = statePtr->pc;
	int decimal = statePtr->mem[pc];
	
	struct int25 instr;	
	instr = getInstruction(decimal);
	evalInstructions(instr, statePtr);
		
	// Update pc
	statePtr->pc++;
	
	return;
}

// Helper to showStates. Prints out all register and memory values at given state.
void printState(stateType *statePtr) {
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

// Print out all memory values at given state.
void printMem(stateType *statePtr) {
	int i;
	for (i=0; i<statePtr->numMemory; i++) {
			printf("memory[%d]=%d\n", i, statePtr->mem[i]);
	}
} 

// Convert a 16-bit number into a 32-bit Linux integer
int
convertNum(int num) {
	if (num & (1<<15) ) {
	    num -= (1<<16);
	}
	return(num);
}