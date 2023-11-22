/**
 * umCommands.h
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * This is the header file to umCommands.c. It declares functions for all 14
 * possible commands to be called in umRun.c. The functions outlined in this 
 * are caleld by umRun and have a 1-1 representation with opcodes found in 
 * umRun.h
 * 
 */

#include "umMemoryAlloc.h"
#include <stdio.h>

void conditionalMove(uint32_t *a, uint32_t *b, uint32_t* c);

void segmentLoad(uint32_t *a, uint32_t *b, uint32_t* c, Memory_T memory);

void segmentStore(uint32_t *a, uint32_t *b, uint32_t* c, Memory_T memory);

void addition(uint32_t *a, uint32_t *b, uint32_t* c);

void multiplication(uint32_t *a, uint32_t *b, uint32_t* c);

void division(uint32_t *a, uint32_t *b, uint32_t* c);

void bitwiseNAND(uint32_t *a, uint32_t *b, uint32_t* c);

/* halt needs to be implemented in run */

void mapSegment(uint32_t *b, uint32_t *c, Memory_T memory);

void unmapSegment(uint32_t* c, Memory_T memory);

void output(uint32_t* c);

void input(uint32_t* c);

void loadProgram(uint32_t *b, uint32_t *c,
        Memory_T memory, uint32_t **programCounter);

void loadValue(uint32_t *a, uint32_t value);



