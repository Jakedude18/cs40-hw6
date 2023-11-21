/**
 * umMemoryAlloc.c
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * Handles deallocation and allocation of dynamic memory. Called by the 
 * umCommands submodule for the map segment, unmap segment, load program, and  * load value commands.
 */
#include "umMemoryAlloc.h"
#include <seq.h>
#include <stdio.h>

struct Memory {
        Seq_T segments;
        /* unmapped segment ids*/
        Seq_T ids;             
};



struct Segment {
        uint32_t * arr;
        int length;
};

typedef struct Segment *Segment_T;



Segment_T initializeSegment(uint32_t *arr, int length){
        Segment_T segment= ALLOC(sizeof(struct Segment));
        segment->arr = arr;
        segment->length = length;
        return segment;
}

void deallocateSegment(Segment_T segment){
    //fprintf(stderr, "array at 0 %u\n", segment->arr[0]);
    FREE(segment->arr);
    FREE(segment);
}




/* constructor */
Memory_T initalizeMemory(uint32_t * initial, int length){
        Memory_T memory = ALLOC(sizeof(struct Memory));
        memory->segments = Seq_new(1);
        memory->ids = Seq_new(0);
        Segment_T firstSegment = initializeSegment(initial, length);
        Seq_T segments = memory->segments;
        Seq_addlo(segments, firstSegment);
        return memory;
}


/* deconstructor */
void deallocateMemory(Memory_T memory){
        Seq_T segments = memory->segments;
        int length = Seq_length(segments);
        for(int i = 0; i < length; i++){
                // fprintf(stderr,"i is %d\n",i);
                
                Segment_T cur = (Segment_T) Seq_remlo(segments);
                deallocateSegment(cur);
        }
        Seq_free(&segments);
        Seq_free(&memory->ids);
        FREE(memory);
}


uint32_t loadMemory(uint32_t length, Memory_T memory){
        Seq_T segments = memory->segments;
        uint32_t * newArr = CALLOC(length, sizeof(uint32_t));
        Segment_T newSeg = initializeSegment(newArr, length);
        // fprintf(stderr, "CALLOC in load memory\n");
        /*figure out 32 bit identifier (index) */
        uint32_t index = Seq_length(segments);
        if(Seq_length(memory->ids) > 0){
                index = (uint32_t)(uintptr_t) Seq_remlo(memory->ids);
                // fprintf(stderr, "using old index\n");
                Segment_T oldVal = (Segment_T) Seq_put(segments, index, newSeg);
                if(oldVal != NULL){
                        // fprintf(stderr, "freeing old val");
                        deallocateSegment(oldVal);
                }
        }
        else
        {
                Seq_addhi(segments, newSeg);
                // fprintf(stderr, "new length is %d\n", Seq_length(segments));
        }
        
        // fprintf(stderr, "return index is %u\n", index);
        return index;
}

void unloadMemory(uint32_t index, Memory_T memory){
        // Seq_T segments = memory->segments;
        uint32_t id = index;
        Seq_addlo(memory->ids, (void *)(uintptr_t)id);
        // Segment_T oldSegment = (Segment_T) Seq_put(segments, index, NULL);
        // deallocateSegment(oldSegment);
        // fprintf(stderr, "memory unloaded\n");
        // uint32_t * segment = (uint32_t *) (Seq_get(segments, index));
        // (void) segment;
}


uint32_t getMemory(uint32_t index, uint32_t offset, Memory_T memory){
        Seq_T segments = memory->segments;
        Segment_T segment = (Segment_T) (Seq_get(segments, index));
        // assert(segment->arr == NULL);
        return segment->arr[offset];
}

void setMemory(uint32_t index, uint32_t offset, uint32_t value, Memory_T memory){
        Seq_T segments = memory->segments;
        //fprintf(stderr, "index is: %u memory length is: %u, offset is: %u\n", index, Seq_length(memory->segments), offset);
        Segment_T segment = (Segment_T) (Seq_get(segments, index));
        // assert(segment->arr == NULL);
        segment->arr[offset] = value;
}

void segmentDuplicate(int index, Memory_T memory){
        // fprintf(stderr, "segmentDuplicateRan\n");
        Seq_T segments = memory->segments;
        Segment_T originalSegment = (Segment_T) (Seq_get(segments, index));
        uint32_t *newArr = CALLOC(originalSegment->length, sizeof(uint32_t)); 
        // fprintf(stderr, "made if half way\n");
        // fprintf(stderr, "length is %d\n", originalSegment->length);
        // fprintf(stderr, "index of duplication is %d\n", index);
        for(int i = 0; i < originalSegment->length; i++){
            newArr[i] = originalSegment->arr[i];
        }
        Segment_T newSegment = initializeSegment(newArr, originalSegment->length);
        // Segment_T oldSegment = (Segment_T) Seq_get(segments, 0);
        Seq_put(segments, 0, newSegment);
        deallocateSegment(Seq_put(segments, 0, newSegment));
        // Segment_T oldSegment = (Segment_T) Seq_put(segments, 0, newSegment);
        // fprintf(stderr, "old segment pointer is %u\n", oldSegment->arr[0]);
        fprintf(stderr, "made it to the end of segmentDuplicate\n");
}
