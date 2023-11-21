/**
 * umIO.c
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * Holds the functions to read instructions in a file into a sequence. Converts
 * each instruction line into the Um_instruction format.
 */


#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>
#include "umIO.h"




Um_instruction getInstruction(FILE *fp){
        
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