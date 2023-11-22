/**
 * umCommands.c
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * Functionality for the 14 possible instructions in um. Commands are called by
 * umRun.c and memory is allocated and freed in umMemoryAlloc.c.
 */


#include "umCommands.h"


void conditionalMove(uint32_t *a, uint32_t *b, uint32_t *c){
        if(*c != 0){
                *a = *b;
        }
}


void segmentLoad(uint32_t *a, uint32_t *b, uint32_t *c, Memory_T memory){
        // fprintf(stderr, "ra is: %u", "rb is: %u", "rc is: %u", *a, *b, *c);
        *a = getMemory(*b, *c, memory);
}

void segmentStore(uint32_t *a, uint32_t *b, uint32_t *c, Memory_T memory){
        setMemory(*a, *b, *c, memory);
}


void addition(uint32_t *a, uint32_t *b, uint32_t *c){
        *a = *b + *c;
}

void multiplication(uint32_t *a, uint32_t *b, uint32_t *c){
        *a = *b * *c;
}

void division(uint32_t *a, uint32_t *b, uint32_t *c){
        assert(*c != 0);
        *a = *b / *c;
}

void bitwiseNAND(uint32_t *a, uint32_t *b, uint32_t *c){
        *a = ~(*b & *c);
}


void mapSegment(uint32_t *b, uint32_t *c, Memory_T memory){
        *b = loadMemory(*c, memory);
}

void unmapSegment(uint32_t *c, Memory_T memory){
        unloadMemory(*c, memory);
}

void output(uint32_t *c){
        uint32_t val = *c % 256;
        printf("%c", val);
}

void input(uint32_t *c){
        *c = fgetc(stdin);
        if ((int)*c == EOF){
                uint32_t all0 = 0;
                *c = ~all0;
        }
}
void loadProgram(uint32_t *b, uint32_t *c,
        Memory_T memory, uint32_t **programCounter){
            
        uint32_t * dupSegAddress = segmentDuplicate(*b, memory);
        *programCounter = dupSegAddress + *c;
}

void loadValue(uint32_t *a, uint32_t value){
        *a = value;
}
