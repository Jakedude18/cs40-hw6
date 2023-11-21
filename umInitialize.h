/**
 * umInitialize.h
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * This is the header file to umInitialize.c. It declares the initialize
 * function that calls umIO.c and the function to copy the sequence created in
 * umIO to an array.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>
#include <mem.h>
#include "umIO.h"


Um_instruction* umInitialize(FILE *fp, int expectedLength);






