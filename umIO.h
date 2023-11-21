/**
 * umIO.h
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * This is the header file to umIO.c. It declares a function to be called in 
 * umInitialize and a helper function to convert file lines into 
 * Um_instructions.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>

typedef uint32_t Um_instruction;


Um_instruction getInstruction(FILE *fp);

