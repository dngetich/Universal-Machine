/*
*
*       load.h
*
* Anahita Sethi and David Ngetich
*
*/

#include <stdio.h>
#include <stdlib.h>
#include "segment.h"
#include "bitpack.h"

#ifndef LOAD_INCLUDED
#define LOAD_INCLUDED

struct segment *load_instruction(int argc, char* argv[], struct segment* instructions);
int load_helper(Seg_T p, uint32_t ID);
#endif