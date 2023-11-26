#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

#define TestDir "testFiles"

extern void Um_write_sequence(FILE *output, Seq_T instructions);

void all_commands_test(Seq_T stream);
void reuse_unloaded_memory(Seq_T stream);
void print_digit_test_no_halt(Seq_T stream);

/* um instruction set tests*/

extern void halt_test(Seq_T stream);
extern void output_test(Seq_T stream);
extern void load_value_test(Seq_T stream);
extern void cmove_test(Seq_T stream);
extern void add_test(Seq_T stream);
extern void mul_test(Seq_T stream);
extern void div_test(Seq_T stream);
extern void nand_test(Seq_T stream);
extern void activate_test(Seq_T stream);
extern void deactivate_test(Seq_T stream);
extern void sstore_test(Seq_T stream);
extern void sload_test(Seq_T stream);
extern void input_test(Seq_T stream);
extern void loadp_test(Seq_T stream);


/* The array `tests` contains all unit tests for the lab. */

static struct test_info {
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*build_test)(Seq_T stream);
} tests[] = {
        { "all_commands",         NULL, "", all_commands_test },
        { "reuse_unloaded_memory", NULL, "", reuse_unloaded_memory },
        { "halt",         NULL, "", halt_test },
        { "output", NULL, "6", output_test },
        { "load_value", NULL, "", load_value_test },
        { "cmov", NULL, "6", cmove_test },
        { "add",  NULL, "6", add_test },
        { "mul", NULL, "6", mul_test },
        { "div", NULL, "6", div_test},
        { "nand", NULL, "6", nand_test},
        { "activate", NULL, "", activate_test},
        { "deactivate", NULL, "", deactivate_test},
        { "sstore", NULL, "", sstore_test},
        { "sload", NULL, "", sload_test},
        { "input", "6", "6", input_test},
        { "loadp", NULL, "6", loadp_test    }
};

  
#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path', 
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test);


int main (int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed; /* failed nonzero == exit nonzero == failure */
}


static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(
                Fmt_string("%s/%s.um", TestDir, test->name));
        Seq_T instructions = Seq_new(0);
        test->build_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s/%s.0", TestDir, test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("%s/%s.1", TestDir,test->name),
                             test->expected_output);
}


static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);

                fputs(contents, input);
                fclose(input);
        }
        free(path);
}


static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}
