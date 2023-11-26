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
#include <sys/stat.h>


#include "umInitialize.h"
#include "umRun.h"




static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [filename]\n",
                        progname);
        exit(1);
}

/**********main********
 * Handles the main functionality of the program. Handles the inputted
 * file, and send it through umInitialize and umRun.
 * 
 * Inputs: 
 *      int argc: the number of arguements given to the program
 *      char *argv[]: contains the file name
 * Return: EXIT_SUCCESS    
 * Notes: fp deallocated in umInitialize.    
 ************************/
int main(int argc, char *argv[])
{
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

    struct stat *output = malloc(sizeof(struct stat));
    stat(argv[1], output);


    /* divide by 4 to get # of um_instructions in file */
    int expectedLength = output->st_size / 4;

    free(output);

    
    Um_instruction *program = umInitialize(fp, (uint32_t)expectedLength);
    
    umRun(program, expectedLength);

    fclose(fp);
    return 0;
}