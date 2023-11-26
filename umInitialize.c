/**
 * umInitialize.c
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * Handles creating an array of all the instructions passed into um.
 * Converts btyecode into umInstrcutions.
 */

#include "umInitialize.h"


#define wordLen 32


Um_instruction getInstruction(FILE *fp);


/**********umInitialize********
* creates an array of Um instructions from a file of instructions
* Inputs:
*       FILE *fp: file pointer of file inputted to um
*       int expectedLength: length of the file
* Returns:
*       Array of the Um_instructions
* Notes:
*       Array is freed in umRun
************************/
Um_instruction* umInitialize(FILE *fp, int expectedLength)
{
        
        assert(fp);
        Um_instruction *program = CALLOC(expectedLength,
                sizeof(Um_instruction));
        for(int i = 0; i < expectedLength; i++) {
                program[i] = getInstruction(fp);
        }
        
        return program;
}

/**********getInstruction********
* bitpacks an instruction into an Um_instruction from reading in values in file
* Inputs:
*       FILE *fp: file pointer of file inputted to um
* Returns:
*       Um_instruction of the instruction created
************************/
Um_instruction getInstruction(FILE *fp)
{
        
        assert(fp);

        uint64_t umInstruction = 0;
        int first = getc(fp);
        int second = getc(fp);
        int third = getc(fp);
        int fourth = getc(fp);

        umInstruction = Bitpack_newu(umInstruction, 8, 24, first);
        umInstruction = Bitpack_newu(umInstruction, 8, 16, second);
        umInstruction = Bitpack_newu(umInstruction, 8, 8, third);
        umInstruction = Bitpack_newu(umInstruction, 8, 0, fourth);


        return umInstruction;
}

