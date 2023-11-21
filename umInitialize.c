/**
 * umInitialize.c
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * Handles creating a sequence of all the instructions passed into um and
 * and loading instructions into memory. Creates c array of the instructions
 * to be called by umRun.c.
 */

#include "umInitialize.h"


#define wordLen 32


Um_instruction* umInitialize(FILE *fp, int expectedLength){
        
        assert(fp);

        /* for testing
        FILE *output = fopen("output.txt", "w");
        Um_write_sequence(output, program);
        fclose(output);
        */
        Um_instruction *program = CALLOC(expectedLength, sizeof(Um_instruction));
        for(int i = 0; i < expectedLength; i++){
                program[i] = getInstruction(fp);
        }
        
        return program;
}


