/* 
* Anahita Sethi and David Ngetich
* Homework #6
* UM
* um.c (main)
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "load.h"
#include "execute.h"
#include "assert.h"
#include "except.h"

int main(int argc, char *argv[]) {
    
    struct segment *instructions = NULL;
    instructions = (struct segment*) malloc(sizeof(struct segment));
    instructions = load_instruction(argc, argv, instructions);
    run_um(instructions);

    return EXIT_SUCCESS; 
}