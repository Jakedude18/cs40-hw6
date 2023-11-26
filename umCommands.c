/**
 * umCommands.c
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * Functionality for the 14 possible instructions in um. Commands are called by
 * umRun.c and memory is allocated and freed in umMemoryAlloc.c.
 */


#include "umCommands.h"


/**********conditionalMove********
* sets register b to register a if register c is equal to 0
* Inputs:
*       uint32_t *a, *b, *c: registers a, b, c
* Returns:
*       void    
************************/
void conditionalMove(uint32_t *a, uint32_t *b, uint32_t *c)
{
        if(*c != 0) {
                *a = *b;
        }
}

/**********segmentLoad********
* gets value in memory at $m[$r[B]][$r[C]] and stores it in register a
* Inputs:
*       uint32_t *a, *b, *c: registers a, b, c
*       Memory_T memory: holds memory for um
* Returns:
*       void    
************************/
void segmentLoad(uint32_t *a, uint32_t *b, uint32_t *c, Memory_T memory)
{
        *a = getMemory(*b, *c, memory);
}

/**********segmentStore********
* gets stores value in register C in memory at $m[$r[A]][$r[B]]
* Inputs:
*       uint32_t *a, *b, *c: registers a, b, c
*       Memory_T memory: holds memory for um
* Returns:
*       void    
************************/
void segmentStore(uint32_t *a, uint32_t *b, uint32_t *c, Memory_T memory)
{
        setMemory(*a, *b, *c, memory);
}

/**********addition********
* sets register a to the sum of registers b and c
* Inputs:
*       uint32_t *a, *b, *c: registers a, b, c
* Returns:
*       void    
************************/
void addition(uint32_t *a, uint32_t *b, uint32_t *c)
{
        *a = *b + *c;
}

/**********multiplication********
* sets register a to the product of registers b and c
* Inputs:
*       uint32_t *a, *b, *c: registers a, b, c
* Returns:
*       void    
************************/
void multiplication(uint32_t *a, uint32_t *b, uint32_t *c)
{
        *a = *b * *c;
}

/**********division********
* sets register a to the quotient of registers b and c
* Inputs:
*       uint32_t *a, *b, *c: registers a, b, c
* Returns:
*       void    
************************/
void division(uint32_t *a, uint32_t *b, uint32_t *c)
{
        assert(*c != 0);
        *a = *b / *c;
}

/**********bitwiseNAND********
* sets register a to the bitwiseNAND of registers b and c
* Inputs:
*       uint32_t *a, *b, *c: registers a, b, c
* Returns:
*       void    
************************/
void bitwiseNAND(uint32_t *a, uint32_t *b, uint32_t *c)
{
        *a = ~(*b & *c);
}

/**********mapSegment********
* calls the loadMemory which creates a new segment with the number of words.
* loadMemory's return value is stored in register b
* equal to the value in register c
* Inputs:
*       uint32_t *b, *c: registers b, c
*       Memory_T memory: holds memory for um
* Returns:
*       void    
************************/
void mapSegment(uint32_t *b, uint32_t *c, Memory_T memory)
{
        *b = loadMemory(*c, memory);
}

/**********unmapSegment********
* calls the unload memory function that unmaps the segment in $m[$r[C]] and
* and stores its identifier to be used for future mapping of segments
* Inputs:
*       uint32_t *c: register c
*       Memory_T memory: holds memory for um
* Returns:
*       void    
************************/
void unmapSegment(uint32_t *c, Memory_T memory)
{
        unloadMemory(*c, memory);
}

/**********output********
* Prints the value of register c as a value between 0 and 255 to stdout
* Inputs:
*       uint32_t *c: register c
* Returns:
*       void    
************************/
void output(uint32_t *c)
{
        uint32_t val = *c % 256;
        printf("%c", val);
}

/**********input********
* reads input into register c. Once end of input is reached, register C is
* set to a 32-bit word where every bit is 1
* Inputs:
*       uint32_t *c: register c
* Returns:
*       void    
************************/
void input(uint32_t *c)
{
        *c = fgetc(stdin);
        if ((int)*c == EOF) {
                uint32_t all0 = 0;
                *c = ~all0;
        }
}

/**********loadProgram********
* replaces program in $m[0] with a new program. Sets programCounter to 
* $m[0][$r[C]].
* Inputs:
*       uint32_t *b, *c: registers b, c
*       Memory_T memory: holds memory for um
*       uint32_t **programCounter: pointer to the programCounter in umRun
* Returns:
*       void    
************************/
void loadProgram(uint32_t *b, uint32_t *c,
        Memory_T memory, uint32_t **programCounter)
{
            
        uint32_t * dupSegAddress = segmentDuplicate(*b, memory);
        *programCounter = dupSegAddress + *c;
}

/**********loadValue********
* sets value to register a
* Inputs:
*       uint32_t *a: register a
*       uint32_t value: value read in umRun to be passed to register a
* Returns:
*       void    
************************/
void loadValue(uint32_t *a, uint32_t value)
{
        *a = value;
}
