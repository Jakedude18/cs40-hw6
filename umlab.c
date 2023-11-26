/*
 * umlab.c
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *  
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.  
 * 
 * Any additional functions and unit tests written for the lab go
 * here. 
 *  
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>

#define wordLen 32
#define registerLen 3
#define opcodeLen 4
#define valueLen 25

typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc);
Um_instruction loadval(unsigned ra, unsigned val);

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc){
        uint32_t word = 0;
        word = Bitpack_newu(word, registerLen, 0, rc);
        word = Bitpack_newu(word, registerLen, registerLen, rb);
        word = Bitpack_newu(word, registerLen, registerLen * 2, ra);
        word = Bitpack_newu(word, opcodeLen, wordLen - opcodeLen, op); 

        return word;
}

Um_instruction loadval(unsigned ra, unsigned val){
        uint32_t word = 0;

        word = Bitpack_newu(word, opcodeLen, wordLen - opcodeLen, LV);
        word = Bitpack_newu(word, registerLen, valueLen, ra);
        word = Bitpack_newu(word, valueLen, 0, val);

        return word;
}


/* Wrapper functions for each of the instructions */

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction cmov(Um_register a, Um_register b, Um_register c){
        return three_register(CMOV, a, b, c);
}

static inline Um_instruction sload(Um_register a,
        Um_register b, Um_register c){
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction sstore(Um_register a, Um_register b,
        Um_register c){
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction mul(Um_register a, Um_register b, Um_register c){
        return three_register(MUL, a, b, c);
}

static inline Um_instruction div(Um_register a, Um_register b, Um_register c){
        return three_register(DIV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c){
        return three_register(NAND, a, b, c);
}

static inline Um_instruction activate(Um_register b, Um_register c){
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction inactive(Um_register c){
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction in(Um_register c){
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction loadp(Um_register b, Um_register c){
        return three_register(LOADP, 0, b, c);
}



/* Functions for working with streams */

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }
      
}


/* Unit tests for the UM */

void all_commands_test(Seq_T stream){

        append(stream, nand(r1, r2, r3));

        append(stream, loadval(r1, 1));


        append(stream, loadval(r7, 48));

        append(stream, add(r2, r7, r2));

        append(stream, output(r2));

        append(stream, loadval(r4, 0));

        append(stream, loadval(r5, 12));

        append(stream, activate(r2, r1));
        
        append(stream, sstore(r2, r4, r5));

        append(stream, sload(r6, r2, r4));

        append(stream, inactive(r2));
        
        append(stream, add(r6, r7, r6));

        append(stream, output(r6));

        append(stream, halt());
}

void reuse_unloaded_memory(Seq_T stream){

        append(stream, loadval(r1, 1));

        append(stream, activate(r2, r1));

        append(stream, inactive(r2));

        append(stream, activate(r2, r1));

        append(stream, loadval(r3, 48));

        append(stream, add(r3, r2, r3));

        append(stream, output(r3));

        append(stream, halt());
}


/* Instruction Set Tests */

void halt_test(Seq_T stream)
{
        append(stream, halt());
}

void output_test(Seq_T stream){
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}


void load_value_test(Seq_T stream){
        append(stream, loadval(r1, 54));
        append(stream, halt());
}

void cmove_test(Seq_T stream){
        append(stream, loadval(r1,1));
        append(stream, loadval(r2, 54));
        append(stream, cmov(r3, r2, r1));
        append(stream, output(r3));
        append(stream, halt());
}

void add_test(Seq_T stream){
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}

void mul_test(Seq_T stream){
        append(stream, loadval(r1, 9));
        append(stream, loadval(r2, 6));
        append(stream, mul(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}

void div_test(Seq_T stream){
        append(stream, loadval(r1, 108));
        append(stream, loadval(r2, 2));
        append(stream, div(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}

/* should be 255 */
void nand_test(Seq_T stream){
        append(stream, nand(r4, r4, r4));
        /* r4 is all 1's */
        append(stream, nand(r4, r4, r4));
        /* r4 is all 0's */
        append(stream, loadval(r3, 54));
        append(stream, add(r4, r3, r4));
        append(stream, output(r3));
        append(stream, halt());
}

void activate_test(Seq_T stream){
        append(stream, loadval(r1, 1));
        append(stream, activate(r2, r1));
        append(stream, halt());
}

void deactivate_test(Seq_T stream){
        append(stream, loadval(r1, 1));
        append(stream, activate(r2, r1));
        append(stream, inactive(r2));
        append(stream, halt());
}

void sstore_test(Seq_T stream){
        append(stream, loadval(r1, 1));
        append(stream, activate(r2, r1));
        append(stream, sstore(r2, r7, r1));
        append(stream, inactive(r2));
        append(stream, halt());
}


void sload_test(Seq_T stream){

        append(stream, loadval(r1, 1));

        append(stream, loadval(r7, 48));

        append(stream, add(r2, r7, r2));

        append(stream, output(r2));

        append(stream, loadval(r4, 0));

        append(stream, loadval(r5, 6));

        append(stream, activate(r2, r1));
        
        append(stream, sstore(r2, r4, r5));

        append(stream, sload(r6, r2, r4));

        append(stream, inactive(r2));
        
        append(stream, add(r6, r7, r6));

        append(stream, output(r6));

        append(stream, halt());
}


void input_test(Seq_T stream){
        append(stream, in(r1));

        append(stream, output(r1));

        append(stream, halt());
}

void loadp_test(Seq_T stream){

        append(stream, loadval(r1, 1));

        append(stream, loadval(r5, 54));

        append(stream, loadval(r6, 5));

        append(stream, loadp(r7, r6));

        append(stream, add(r5, r1, r5));

        append(stream, output(r5));

        append(stream, halt());
}

