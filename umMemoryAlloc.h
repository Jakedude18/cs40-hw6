/**
 * umMemoryAlloc.h
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * This is the header file to umMemoryAlloc.h. Declares a function for
 * allocating memory and one for deallocating memory. Functions to be called
 * in umCommands.c.
 */


#include <uarray.h>
#include <assert.h>
#include <stdint.h>
#include <seq.h>
#include <mem.h>


typedef struct Memory *Memory_T;

Memory_T initalizeMemory(uint32_t * initial, int length);

void deallocateMemory(Memory_T memory);

uint32_t loadMemory(uint32_t length, Memory_T memory);

void unloadMemory(uint32_t index, Memory_T memory);

uint32_t getMemory(uint32_t index, uint32_t offset, Memory_T memory);

void setMemory(uint32_t index, uint32_t offset, uint32_t value, Memory_T memory);

uint32_t * segmentDuplicate(int index, Memory_T memory);