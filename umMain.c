/**
 * umMemoryAlloc.c
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * Main functionality for um. Takes in a file of instructions, reads through
 * them, and executes them. Output is printed to stdout.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "umInitialize.h"
#include "umRun.h"





static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [filename]\n",
                        progname);
        exit(1);
}


int main(int argc, char *argv[]){
    FILE *fp = NULL;


    if(argc == 2) {
        fp = fopen(argv[1], "r");
        if(fp == NULL){
            printf("could not open file: %s\n", argv[1]);
            exit(1);
        }
    }
    else {
        usage(argv[0]);
    }

    Um_Instruction program[] = umInitialize(fp);
    umRun(progam);

    return 0;
}