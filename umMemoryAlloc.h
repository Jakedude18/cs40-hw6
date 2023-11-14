/**
 * umMemoryAlloc.h
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * This is the header file to umMemoryAlloc.h. Declares a function for
 * allocating memory and one for deallocating memory. Functions to be called
 * in umCommands.c.
 */




uint32_t loadMemeory(int size);

void unloadMemeory(uint32_t address);