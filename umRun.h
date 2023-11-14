/**
 * umRun.h
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * This is the header file to umRun.h. Declares a function to loop through each 
 * each instruction and one to free memory and exit code.
 */

typedef uint32_t Um_instruction;
typedef uint32_T

int *register[8];

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

void umRun(Um_Instruction[] program);