/**
 * umRun.h
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * This is the header file to umRun.h. Declares a function to loop through each 
 * each instruction and one to free memory and exit code.
 */

#include "umCommands.h"
#include <bitpack.h>

void umRun(uint32_t *program, uint32_t length);
