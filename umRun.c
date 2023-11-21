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


static void depackWord(uint32_t word, uint32_t **registers, uint32_t **ra, 
        uint32_t **rb, uint32_t **rc, Um_opcode *op);
static void executeCommand(uint32_t *ra, uint32_t *rb, uint32_t *rc,
        Um_opcode op, Memory_T memory, uint32_t *programCounter);

static void executeLoadValue(uint32_t instruction, uint32_t **registers);

static void deallocateRegisters(uint32_t** registers);



/* frees program*/
void umRun(uint32_t *program, uint32_t length){
        uint32_t programCounter = 0;

        uint32_t** registers = ALLOC(sizeof(uint32_t *) * registersLen);
        for(int i = 0; i < registersLen; i++){
                registers[i] = CALLOC(1, sizeof(uint32_t));
        }

        /* initialize memoery */
        Memory_T memory = initalizeMemory(program, length);
        

        while(programCounter < length){
                uint32_t *ra, *rb, *rc;
                // *rc = ALLOC(sizeof(uint32_t));
                Um_opcode op;
                depackWord(program[programCounter], registers, &ra, &rb, &rc, &op);
                
                // fprintf(stderr, "programCounter is %u\n", programCounter);

                if(op == LV){
                        executeLoadValue(program[programCounter], registers);
                }
                else if(op == HALT){
                        break;    
                }
                else{
                        executeCommand(ra, rb, rc, op, memory, &programCounter);
                }
                if(op != LOADP){
                        programCounter++;
                }
        }

        deallocateRegisters(registers);
        deallocateMemory(memory);
}

static void deallocateRegisters(uint32_t** registers){
        for(int i = 0; i < registersLen; i++){
                FREE(registers[i]);
        }
        FREE(registers);
}



static void executeLoadValue(uint32_t instruction, uint32_t **registers){

        uint32_t *ra = registers[Bitpack_getu(instruction, registerLen, valueLen)];
        uint32_t value = Bitpack_getu(instruction, valueLen, 0);

        loadValue(ra, value);
}

static void depackWord(uint32_t word, uint32_t **registers, uint32_t **ra, 
        uint32_t **rb, uint32_t **rc, Um_opcode *op)
{

        *rc = registers[Bitpack_getu(word, registerLen, 0)];
        *rb = registers[Bitpack_getu(word, registerLen, registerLen)];
        *ra = registers[Bitpack_getu(word, registerLen, registerLen * 2)];

        *op = Bitpack_getu(word, opcodeLen, wordLen - opcodeLen);

}

static void executeCommand(uint32_t *ra, uint32_t *rb, uint32_t *rc,
        Um_opcode op, Memory_T memory, uint32_t *programCounter)
{        
        switch(op){
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
                case LOADP: loadProgram(rb, memory);
                        *programCounter = *rc;
                        break;
                case LV: break;
        }
}

