/*
*       Comp40 UM/HW6
*         load.c
*
* Anahita Sethi and David Ngetich
*
*/

#include "segment.h"
#include "load.h"
#include "bitpack.h"
#include "seq.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mem.h>
#include <assert.h>
 
/* 
 * name: load_helper
 * parameter: Seg_T, uint32_t
 * return: int
 * Does: helper function for load program function in execute.c 
 */
int load_helper(Seg_T p, uint32_t ID)
{
              
        struct segment *curr_program = Seq_get(p, ID);
        uint32_t prog_length = curr_program->seg_len;

        /* make another program setting the segment length
         * to equal that of the intial program length
         */
        struct segment *new_segment = malloc(sizeof(struct segment));
        new_segment->seg = malloc(prog_length * sizeof(uint32_t));
        new_segment->seg_len = prog_length;
        for (unsigned i = 0; i < prog_length; i++) {
                new_segment->seg[i] = curr_program->seg[i];
        }

        /*remove the old program */
        struct segment *old_program = Seq_get(p, 0);
        free(old_program->seg);

         /* put the new program into segment zero */
         Seq_put(p, 0, new_segment);
        
         return prog_length;
}

/* 
 * name: load_instructions
 * parameter: int, char*
 * return: segment*
 * Does: Reads the binary from .um file passed in and convert 
 *       the it to 32-bit word using Bitpack. 
 */
struct segment* load_instruction(int argc, char* argv[], 
    struct segment *instructions)
{

        

       FILE *fp;
        struct stat file;
        int instruction_len;

        assert(argc >=  2);

        stat(argv[1], &file);
        int size = file.st_size; 

        //fprintf(stderr, "size is:%d\n", size); 
        if (size % 4 != 0 || size == 0) {
            fprintf(stderr, "Incorrect file\n");
            exit(EXIT_FAILURE);
        } 

        instruction_len = size / 4;
        int *instruction = malloc(instruction_len * sizeof(uint32_t));

       fp = fopen(argv[1], "rb");

        /*check to ensure the file is not null */
        //assert(fp != NULL);
        if (fp == NULL) {
                fprintf(stderr, "Error opening file.\n");
                exit(EXIT_FAILURE);
        }

        /*read in all instructions */
        uint32_t instr = 0;
        for (int i = 0; i < instruction_len; i++) {
                for (unsigned j = 24; (int)j >= 0; j -= 8){
                        int temp = getc(fp);

                        /* check for enough binary to read */
                        if (temp == EOF) { 
                                fprintf(stderr, "Insufficient words.\n");
                                exit(0);
                        }       
                        instr = Bitpack_newu(instr, 8, j, temp);
                }

                instruction[i] = instr;
        }

        /*populate the segment with the instructions */
        fclose(fp);
        instructions->seg = instruction;
        instructions->seg_len = instruction_len;
        return instructions;
        free(instruction);   
}