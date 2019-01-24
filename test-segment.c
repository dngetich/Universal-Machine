/*
*
* Anahita Sethi and David Ngetich
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#include <assert.h>
#include "test-segment.h"


static bool PASS = 1;

int main(int argc, char *argv[])
{
        (void)argv;
        (void)argc;

        bool passed = true;

        passed =  (test_new_free() && test_get_instruction() && 
                test_map_unmap());

        printf("The segmented memory will %s pass\n", (passed ? "" : "NOT"));

        return EXIT_SUCCESS;
}

bool test_new_free(void){
        Seg_T seg  = seg_new();
        seg_free(&seg);
        PASS = (seg == NULL);
        printf("New and free and will %s pass\n", (PASS ? "" : " NOT"));
        return PASS;
}

bool test_get_instruction(void){
        Seg_T seg = seg_new();

        /*create simple prog */
        int runProgram[4] = {0x00001111, 0x11112222, 0x22223333, 0x11112223};

        map_seg(seg, 0, 4);
        for(unsigned i = 0; i < 4; i++){
                /*load instruction */
                seg_put(seg, i, runProgram[i]);
        }

        for (unsigned i = 0; i < 4; i++) {
                PASS = (get_instruction(seg,i) == (uint32_t)runProgram[i]);
        }

        unmap_seg(seg, 0);
        seg_free(&seg);
        printf("Getting instructions will %sPASS!\n", (PASS ? "" : "NOT "));
        return PASS;

}

bool test_map_unmap(void)
{
        Seg_T seg = seg_new();
        for (unsigned i = 0; i < 20; i++) {
                map_seg(seg, i, i+1);
                /* check if the mapped segment is of correct length*/
                /* initial step mapped segment  in
                   the ith element in memory*/
                PASS = (seg_length(Seq_get(seg,i)) == i+1);
                unmap_seg(seg, i);
                PASS = (Seq_get(seg,i) == NULL);
        }
        seg_free(&seg);
        printf("Map and Unmap will %sPASS!\n", (PASS ? "" : "NOT "));
        return PASS;
}

















