/*
*
* Anahita Sethi and David Ngetich
*
*/

#ifndef SEGMENT_INCLUDED
#define SEGMENT_INCLUDED

#include <seq.h>
#include <stdlib.h>
#include <stdint.h>

typedef Seq_T Seg_T;

struct segment {
        int* seg;
        unsigned seg_len;
};

/* Initialize the sequence of segment memory */
Seg_T seg_new();

/* frees the allocated mapped segments */
void seg_free(Seg_T* p);

/* puts a value into the memory at passed in index */
void seg_put(Seg_T p, uint32_t ID, uint32_t offset, uint32_t val);

/* maps a segment and then returns the ID of that segment */
uint32_t map_seg(Seg_T p, uint32_t ID, uint32_t size);

/* unmaps the requested segment by deleting it */
void unmap_seg(Seg_T p, uint32_t ID);

/* gets the value segment in the given ID in the sequence */
uint32_t get_value(Seg_T p, uint32_t ID, uint32_t offset);

/* returns a struct pointer to the given segment ID */
struct segment* get_address(Seg_T p, uint32_t ID);

/* returns the length of the segment in memory */ 
int seg_length(Seg_T p);

#endif