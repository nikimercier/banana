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
	noop,
	fill
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
void evalInstruction(struct int25, stateType *);
void printState(stateType *);
void printMem(stateType *);
int convertNum(int);
void getValues(int, struct int25, int *, int *, int *, int *);

int
main(int argc, char *argv[])
{
	char line[MAXLINELENGTH];
	stateType state;
	FILE *filePtr;
	int i, endState, count;

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
		
		// Save halt pc
		if (state.mem[state.numMemory] == 25165824) {
			endState = state.numMemory;
		}
	}

	// Show memory
	printMem(&state);
	
	// Zero all registers
	for (i = 0; i < NUMREGS; i++) {
		state.reg[i] = 0;
	}
	
	count = 0;
	
	// Evaluate and print each state
	while (state.pc != endState + 1) {
		count++;
		printState(&state);
		evalState(&state);
	}

	printf("total of 17 instructions executed\n");
	printf("final state of machine:\n");
	printState(&state);

	return(0);
}

// Return decimal to bin if not .fill. If .fill, return as signed int.
struct int25 getInstruction(int decimal) {
	struct int25 instr;
	instr.data = 0;
	
	// The opcode is 524288 (add 1 0 0) or higher
	if (decimal >= 524288) {
		instr.data = decimal;
	}
	// Otherwise it's a .fill
	else {
		instr.signedData = decimal;
	}
	
	return instr;
}

// Helper to evalInstruction. Returns regA, regB, destReg, offsetField.
void getValues(int opcode, struct int25 instr, int *regA, int *regB, int *destReg, int *offsetField) {
	// R-type instructions (add, nand):
	//     bits 24-22: opcode
	//     bits 21-19: reg A
	//     bits 18-16: reg B
	//     bits 15-3:  unused (should all be 0)
	//     bits 2-0:   destReg
	
	// I-type instructions (lw, sw, beq):
	//     bits 24-22: opcode
	//     bits 21-19: reg A
	//     bits 18-16: reg B
	//     bits 15-0:  offsetField (a 16-bit, 2's complement number with a range of
	//                     -32768 to 32767)
	
	// J-type instructions (jalr):
	//     bits 24-22: opcode
	//     bits 21-19: reg A
	//     bits 18-16: reg B
	//     bits 15-0:  unused (should all be 0)
	
	// O-type instructions (halt, noop):
	//     bits 24-22: opcode
	//     bits 21-0:  unused (should all be 0)
	
	struct int25 temp;
	struct int16 num16;
	
	switch(opcode) {
		// R-type
		case add:
		case nand:
			// Get regA
			temp.data = instr.data;
			temp.data = temp.data << 3;
			temp.data = temp.data >> (3 + 19);
			*regA = temp.data;
			
			// Get regB
			temp.data = instr.data;
			temp.data = temp.data << 6;
			temp.data = temp.data >> (6 + 16);
			*regB = temp.data;
			
			// Get destReg
			temp.data = instr.data;
			temp.data = temp.data << 22;
			temp.data = temp.data >> 22;
			*destReg = temp.data;
			
			// printf("Ur regA is %d and ur regB %d is and ur destReg is %d.\n", *regA, *regB, *destReg);
			break;
		
		// I-type
		case lw:
		case sw:
		case beq:
			// Get regA
			temp.data = instr.data;
			temp.data = temp.data << 3;
			temp.data = temp.data >> (3 + 19);
			*regA = temp.data;
			
			// Get regB
			temp.data = instr.data;
			temp.data = temp.data << 6;
			temp.data = temp.data >> (6 + 16);
			*regB = temp.data;
			
			// Get offsetField
			temp.data = instr.data;
			// Check if 16-bit bin is two's complement negative
			temp.data = temp.data << 9;
			temp.data = temp.data >> 9;
			if (temp.data >> 15) {
				// Left-most bit is 1. Is negative.
				num16.data = temp.data;
				num16.data = ~temp.data + 1;

				*offsetField = num16.data;
				*offsetField *= -1;
			}
			else {
				*offsetField = temp.data;
			}
			
			// printf("Ur regA is %d and ur regB %d is and ur offsetField is %d.\n", *regA, *regB, *offsetField);
			break;
			
		// J-type
		case jalr:
			// Get regA
			temp.data = instr.data;
			temp.data = temp.data << 3;
			temp.data = temp.data >> (3 + 19);
			*regA = temp.data;
			
			// Get regB
			temp.data = instr.data;
			temp.data = temp.data << 6;
			temp.data = temp.data >> (6 + 16);
			*regB = temp.data;
			
			// printf("Ur regA is %d and ur regB %d.\n", *regA, *regB);
			break;
			
		// halt, noop, .fill
		case halt:
		case noop:
		// .fill
		default:;
	}
}

// Evaluate instruction
void evalInstruction(struct int25 instr, stateType *state) {
	int opcode, regA, regB, destReg, offsetField, *reg, *mem;
	reg = state->reg;
	mem = state->mem;

	// Get opcode
	opcode = instr.data ? instr.data >> 22 : fill;
	
	// Get regA, regB, destReg, offsetField
	getValues(opcode, instr, &regA, &regB, &destReg, &offsetField);
	
	switch(opcode) {
		// R-type
		case add:
			reg[destReg] = reg[regA] + reg[regB];
			break;
		case nand:
			reg[destReg] = ~(reg[regA] & reg[regB]);
			break;
			
		// I-type
		case lw:
			reg[regB] = mem[reg[regA] + offsetField];
			break;
		case sw:
			mem[regA + offsetField] = reg[regB];
			break;
		case beq:
			if (reg[regA] == reg[regB]) {
				state->pc += offsetField;
			}
			break;
			
		// J-type
		case jalr:
			reg[regB] = state->pc + 1;
			state->pc = reg[regA];
			break;
		
		// Other
		case halt:
			printf("machine halted\n");
			break;
		case noop:
			break;
		// .fill
		default:;
	}

	state->pc++;
}

// Evaluate given state.
void evalState(stateType *statePtr) {
	int pc = statePtr->pc;
	int decimal = statePtr->mem[pc];
	
	struct int25 instr;	
	instr = getInstruction(decimal);
	evalInstruction(instr, statePtr);
	
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
int convertNum(int num) {
	if (num & (1<<15) ) {
	    num -= (1<<16);
	}
	return(num);
}