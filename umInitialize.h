/**
 * umInitialize.h
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * This is the header file to umInitialize.c. It declares the initialize
 * function which takes in a file poitner and expected length and 
 * extracts and returns an array of Um_instructions
 */

#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>
#include <mem.h>

typedef uint32_t Um_instruction;

Um_instruction* umInitialize(FILE *fp, int expectedLength);






