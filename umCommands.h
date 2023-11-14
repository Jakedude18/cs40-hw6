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

#include "umRun.h"

void conditionalMove(int *a, int *b, int* c);

void segementLoad(int *a, int *b, int* c);

void segementStore(int *a, int *b, int* c);

void addition(int *a, int *b, int* c);

void multiplication(int *a, int *b, int* c);

void division(int *a, int *b, int* c);

void bitwiseNAND(int *a, int *b, int* c);

void halt(int *a, int *b, int* c);

void mapSegement(int *a, int *b, int* c);

void unmapSegement(int *a, int *b, int* c);

void output(int *a, int *b, int* c);

void input(int *a, int *b, int* c);

void loadProgram(int *a, int *b, int* c);

void loadValue(int *a, int val);



