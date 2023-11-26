/**
 * umRun.c
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * Handles most of the functionality of the program once called by umMain.c.
 * Receives an array of the instructions from umInitialize.c and loops through
 * and executes each instruction. Additionally, frees all memory and exits
 * code.
 */

#include "umRun.h"
#include <stdio.h>


#define wordLen 32
#define registerLen 3
#define opcodeLen 4
#define valueLen 25
#define registersLen 8


typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


void depackWord(uint32_t word, uint32_t **registers, uint32_t **ra, 
        uint32_t **rb, uint32_t **rc, Um_opcode *op);
void executeCommand(uint32_t *ra, uint32_t *rb, uint32_t *rc,
        Um_opcode op, Memory_T memory, uint32_t **programCounter);

void executeLoadValue(uint32_t instruction, uint32_t **registers);

void deallocateRegisters(uint32_t** registers);

/**********umRun********
* intializes registers and memory for um. Runs while loop for every instruction
* in the given program.
* Inputs:
*       uint32_t *program: array of the instructions of the program passed to 
*               um.
*       uint32_t length: length of the array of instructions.
* Returns:
*       void  
* Notes: registers deallocated in deallocate registers and memory deallocated 
* in deallocateMemory. programCounter jumps values if loadProgram is called.
************************/
void umRun(uint32_t *program, uint32_t length)
{
        uint32_t *programCounter = program;

        uint32_t** registers = ALLOC(sizeof(uint32_t *) * registersLen);
        for(int i = 0; i < registersLen; i++){
                registers[i] = CALLOC(1, sizeof(uint32_t));
        }

        /* initialize memoery */
        Memory_T memory = initalizeMemory(program, length);

        while(*programCounter) {

                uint32_t *ra, *rb, *rc;
                Um_opcode op;

                depackWord(*programCounter, registers, &ra, &rb, &rc, &op);


                if(op == LV) {
                        executeLoadValue(*programCounter, registers);
                }
                else if(op == HALT) {
                        break;    
                }
                else {
                        executeCommand(ra, rb, rc, op, memory, &programCounter);
                }
                if(op != LOADP) {
                        programCounter++;
                }
        }

        deallocateRegisters(registers);
        deallocateMemory(memory);
}

/**********deallocateRegisters********
* frees values held in the registers.
* Inputs:
*       uint32_t **registers: pointer to the array of registers.
* Returns:
*       void
************************/
void deallocateRegisters(uint32_t** registers)
{
        for(int i = 0; i < registersLen; i++) {
                FREE(registers[i]);
        }
        FREE(registers);
}


/**********executeLoadValue********
* executes the loadValue command that sets a 25 bit value a register.
* Inputs:
*       uint32_t instruction: instruction with opcode 13 (load value)
*       that contains the registers to hold the value and the value itself.
*       uint32_t **registers: pointer to the array of registers.
* Returns:
*       void
************************/
void executeLoadValue(uint32_t instruction, uint32_t **registers)
{
        uint32_t *ra = registers[
                Bitpack_getu(instruction, registerLen, valueLen)];
        uint32_t value = Bitpack_getu(instruction, valueLen, 0);

        loadValue(ra, value);
}

/**********depackWord********
* depacks an instruction to retrieve three registers and an opcode.
* Inputs:
*       uint32_t word: instruction to be depacked.
*       uint32_t **registers: pointer to the array of registers
*       uint32_t **ra, **rb, **rc: pointers to pointers
*       that will be pointed to registers a, b and c.
*       Um_opcode *op: points to value of opcode in the word
* Returns:
*       void
************************/
void depackWord(uint32_t word, uint32_t **registers, uint32_t **ra, 
        uint32_t **rb, uint32_t **rc, Um_opcode *op)
{

        *rc = registers[Bitpack_getu(word, registerLen, 0)];
        *rb = registers[Bitpack_getu(word, registerLen, registerLen)];
        *ra = registers[Bitpack_getu(word, registerLen, registerLen * 2)];
        uint32_t opcode =  Bitpack_getu(word, opcodeLen, wordLen - opcodeLen);
        assert(opcode <= 13);
        *op = opcode;

}

/**********executeCommand********
* takes in an instruction and executes it accordingly.
* Inputs:
*       uint32_t *ra, *rb, *rc: pointers to the three registers.
*       Um_opcode op: the opcode for the current command.
*       Memory_T memory: holds memory for um.
*       uint32_t **programCounter: pointer to programCounter in umRun function.
* Returns:
*       void
* Notes: does not handle halt and loadValue commands
************************/
void executeCommand(uint32_t *ra, uint32_t *rb, uint32_t *rc,
        Um_opcode op, Memory_T memory, uint32_t **programCounter)
{        
        switch(op) {
                case CMOV: conditionalMove(ra, rb, rc);
                        break;
                case SLOAD: segmentLoad(ra, rb, rc, memory);
                        break;
                case SSTORE: segmentStore(ra, rb, rc, memory);
                        break;
                case ADD: addition(ra, rb, rc);
                        break;
                case MUL: multiplication(ra, rb, rc);
                        break;
                case DIV: division(ra, rb, rc);
                        break;
                case NAND: bitwiseNAND(ra, rb, rc);
                        break;
                case HALT: break;
                case ACTIVATE: mapSegment(rb, rc, memory);
                        break;
                case INACTIVATE: unmapSegment(rc, memory);
                        break;
                case IN: input(rc);
                        break;
                case OUT: output(rc);
                        break;
                case LOADP: loadProgram(rb, rc, memory, programCounter);
                        break;
                case LV: break;
        }
}

