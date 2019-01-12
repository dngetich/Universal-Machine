/*
*
* execute.c
*
* Anahita Sethi and David Ngetich
*
*/

#include <stdio.h>
#include <stdlib.h>
#include "segment.h"
#include "execute.h"
#include "bitpack.h"
#include "load.h"
#include <seq.h>
#include <mem.h>
#include <assert.h>

/* array of 8 registers */ 
uint32_t R[8] = {0}; 

/* program counter */
static int prog_counter = 0;

static inline void c_move(uint32_t a, uint32_t b, uint32_t c)
{
	if(R[c] != 0) {
		R[a] = R[b];
	}
}

/* get user value in segment at index requested */ 
static inline void seg_load(Seg_T s, uint32_t a, uint32_t b, uint32_t c)
{
	
	R[a] = get_value(s, R[b], R[c]); 
}

static inline void seg_store(Seg_T s, uint32_t a, uint32_t b, uint32_t c)
{
    seg_put(s, R[a], R[b], R[c]);
}

static inline void add(uint32_t a, uint32_t b, uint32_t c)
{ 
	R[a] = (R[b] + R[c]) % 4294967296; /* mod 2 ^ 32 */ 
}

static inline void multiply(uint32_t a, uint32_t b, uint32_t c)
{
	R[a] = (R[b] * R[c]) % 4294967296; /* mod 2 ^ 32 */ 
}

static inline void divide(uint32_t a, uint32_t b, uint32_t c)
{
	R[a] = R[b] / R[c];
}

static inline void nand(uint32_t a, uint32_t b, uint32_t c)
{
	R[a] = ~(R[b] & R[c]);
}

/* halt the program and free all the memory */
static inline void halt(Seg_T s, Stack_T unmap, struct segment* prog)
{	
	/*unmap all the mapped segments*/
        int len = seg_length(s);
        for(int i =0; i < len; i++){
                if(get_address(s, i) != NULL)
                        unmap_seg(s, i);
        }

        /*free and exit */
        Stack_free(&unmap);
        seg_free(&s);
        free(prog); 
        exit(EXIT_SUCCESS);
}

/* map segement by calling the map_seg function */
static inline void map_segment(Seg_T s, Stack_T unmap, uint32_t b, uint32_t c)
{
        if(Stack_empty(unmap)){
                R[b] = map_seg(s, seg_length(s), R[c]);
        } else {
                /*else get unmapped and map*/
                uint32_t id = (uint32_t)(uintptr_t)Stack_pop(unmap);
                R[b] = map_seg(s, id, R[c]);

        }
}

static inline void unmap_segment(Seg_T s, Stack_T unmap, uint32_t c)
{
        unmap_seg(s, R[c]);
        Stack_push(unmap, (void *)(uintptr_t)R[c]);
}

static inline void output(uint32_t c)
{
	/* Checks that output is less than 255 */ 
	assert (R[c] <= 255); 
	putchar(R[c]); 
}

static inline void input(FILE *fp, uint32_t c)
{
    /* Reads from standard input */ 
	uint32_t instruction = getc(fp);
	if (instruction == (uint32_t)EOF) {
		instruction = ~0; 
	}
    assert(instruction <= 255 && instruction > 0);
	R[c] = instruction; 
}

static inline void load_prog(Seg_T s, int* prog_length, uint32_t b, uint32_t c) 
{
	if(R[b] == 0) {
                prog_counter = R[c];
    }
    else {
              
        struct segment *curr_program = Seq_get(s, 0);
        uint32_t prog_len = curr_program->seg_len;

        /* make another program setting the segment length
         * to equal that of the intial program length
         */
        struct segment *new_segment = malloc(sizeof(struct segment));
        new_segment->seg = malloc(prog_len * sizeof(uint32_t));
        new_segment->seg_len = prog_len;
        for (unsigned i = 0; i < prog_len; i++) {
                new_segment->seg[i] = curr_program->seg[i];
        }

        /* remove the old program */
        struct segment *old_program = Seq_get(s, 0);
        free(old_program->seg);

         /* put the new program into segment zero */
         Seq_put(s, 0, new_segment);

           *prog_length = prog_len;
            prog_counter = R[c] - 1;
        }

}

static inline void load_val(uint32_t a, uint32_t val)
{
	R[a] = val; 
}

static inline uint32_t get_reg_opcode(uint32_t word, unsigned width,
    unsigned lsb) 
{
	return(word << (32 - width -lsb)) >> (32 - width);
}

static inline uint32_t get_instruction(Seg_T s, uint32_t ID, uint32_t offset)
{
	struct segment* temp = Seq_get(s, ID);
        return temp->seg[offset];  
}


/*Func: run_um
 *Parameters: Struct segment *
 *Return:Nothing
 *Does: Executes all the instruction after getting the opcode
*/
void run_um(struct segment *prog)
{
        /*getting the um */
        Seg_T mem = seg_new();
        Stack_T unmapIDs = Stack_new();
        int program_len = prog->seg_len;
        uint32_t a,b,c;
        Seq_addhi(mem, prog);	  

        while (prog_counter < program_len)
        {
                uint32_t instruction = get_instruction(mem, 0, prog_counter);
                uint32_t opcode = get_reg_opcode(instruction, 4, 28);

                fprintf(stderr, "Opcode is: %d\n", opcode);
                /*get the values for a, b, and c */
                a = get_reg_opcode(instruction, 3, 6);
                b = get_reg_opcode(instruction, 3, 3);
                c = get_reg_opcode(instruction, 3, 0);
                uint32_t val = get_reg_opcode(instruction, 25, 0);

        	/*switch statement */
        	switch(opcode) {
        		case 0: 
        			c_move(a, b, c); 
        			break; 
        		case 1: 
        			seg_load(mem, a, b, c); 
        			break; 
        		case 2:
                    seg_store(mem, a, b, c);
        			break; 
        		case 3:
        			add(a, b, c); 
        			break; 
        		case 4: 
        			multiply(a, b, c); 
        			break; 
        		case 5:
        			divide(a, b, c); 
        			break; 
        		case 6:
        			nand(a, b, c); 
        			break; 
        		case 7:
                    halt(mem, unmapIDs, prog); 
        			break; 
        		case 8: 
        			map_segment(mem, unmapIDs, b, c); 
        			break; 
        		case 9: 
        			unmap_segment(mem, unmapIDs, c); 
        			break; 
        		case 10: 
        			output(c); 
        			break; 
        		case 11: 
        			input(stdin, c); 
        			break; 
        		case 12:
        			load_prog(mem, &program_len, b, c); 
        			break; 
        		case 13:
                    a = get_reg_opcode(instruction, 3, 25); 
        			load_val(a, val); 
        			break; 
        	}
            prog_counter++;
        }
}