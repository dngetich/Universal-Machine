/*
*       Comp40 UM/HW6
*         segment.c
*
* Anahita Sethi and David Ngetich
*
* segment.c: Contains the implementation of the functions for the segmented
        memory. We are using a sequences of segments 
*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "segment.h"
#include <seq.h>
#include <mem.h>
#include <assert.h>

const int HINT = 10;

/* Initialize the sequence of segment memory */
Seg_T seg_new()
{
        return Seq_new(HINT); 
}
/* frees the allocated mapped segments */
void seg_free(Seg_T* p)
{
	Seq_free(p);
}

/*Func: map_seg
 *Parameters: Seg_T, uint32_t, uint32_t
 *return: uint32_t
 * Does: maps a segment and then returns the ID of that segment 
 */
uint32_t map_seg(Seg_T p, uint32_t ID, uint32_t size)
{
        /*if the ID does not exist then map a new segment */
        if((int)ID >= seg_length(p)) {

                /* add a new segment to the sequence of segments */
                struct segment *new_segment = malloc(sizeof(struct segment));
                new_segment->seg = malloc(size * sizeof(uint32_t));
                new_segment->seg_len = size;
          
                /* initialize the segments to 0 */
                for (unsigned i = 0; i < size; i++) {
                        new_segment->seg[i] = 0;
                }
                Seq_addhi(p, new_segment);
                int seq_len = Seq_length(p);
                free(new_segment); 
                return (seq_len - 1);

        /* else if ID exists then reuse the segments that have been unmapped */
        } else {
                struct segment *new_segment = malloc(sizeof(struct segment));
                new_segment->seg = malloc(size * sizeof(uint32_t));
                new_segment->seg_len = size;

                /* initialize the segments to 0 */
                for (unsigned i = 0; i < size; i++) {
                        new_segment->seg[i] = 0;
                }

                Seq_put(p, ID, new_segment);
                free(new_segment); 
                return ID;
        }
}

/*Func: map_seg
 *Parameters: Seg_T, uint32_t, uint32_t
 *return: Nothing
 *Does: unmaps the requested segment by deleting it 
 */ 
void unmap_seg(Seg_T p, uint32_t ID)
{
	
        struct segment *temp = Seq_get(p, ID);
        free(temp->seg);
        Seq_put(p, ID, NULL);
}

/*
 *Func: seg_length
 *parameters:Seg_T
 *Return: int
 *Does: returns the length of the segment in memory 
 */ 
int seg_length(Seg_T p)
{
	return Seq_length(p); 
}

/*
 *Func: seg_get
 *parameters: Seg_T, uint32_t, uint32_t
 *Return: uint32_t
 *Does: gets the instruction in the segment of the given ID in the sequence 
*/
uint32_t get_value(Seg_T p, uint32_t ID, uint32_t offset)
{
        struct segment *curr = Seq_get(p, ID);
        return curr->seg[offset];
}

/*
 *Func: get_address
 *parameters: Seg_T, uint32_t
 *Return: struct Segment
 *Does: returns a struct pointer to the given segment ID
*/
struct segment* get_address(Seg_T p, uint32_t ID)
{
        return ((struct segment*)Seq_get(p, ID));

}

/*
 *Func: seg_put
 *parameters: Seg_T, uint32_t, uint32_t, uint32_t
 *Return: Nothing
 *Does: puts a value into the memory at passed in index
*/
void seg_put(Seg_T p, uint32_t ID, uint32_t offset, uint32_t value)
{
        //fprintf(stderr,"In seg put\n"); 
        //p = Seq_get(p, ID);
        //fprintf(stderr,"Called seq get\n"); 
        //Seq_put(p, offset, (void*)(uintptr_t)value); 
        
        //fprintf(stderr,"Updated temp\n"); 



        /************** 1 **********/
        struct segment *temp = get_address(p, ID);
        (temp->seg)[offset] = value;
	
        /*************** 2 ********
        struct segment *temp = Seq_get(p, ID);
        (temp->seg)[offset] = value;


        ************** 3 **********
        get_value(p, ID, offset) = value;
	*/

}  
