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
    // fprintf(stderr,"new segment arr is %p\n", (void *)segment->arr);
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
                        //fprintf(stderr, "freeing old val");
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
        assert(segment->arr != NULL);
        return segment->arr[offset];
}

void setMemory(uint32_t index, uint32_t offset, uint32_t value, Memory_T memory){
        Seq_T segments = memory->segments;
        Segment_T segment = (Segment_T) (Seq_get(segments, index));
        assert(segment->arr != NULL);
        // fprintf(stderr, "index is: %u arr length is: %d, offset is: %u\n", index, segment->length, offset);
        segment->arr[offset] = value;
}

uint32_t * segmentDuplicate(int index, Memory_T memory){
        // fprintf(stderr, "segmentDuplicateRan\n");
        Seq_T segments = memory->segments;
        
        Segment_T originalSegment = (Segment_T) (Seq_get(segments, index));
        if(index == 0){
            return originalSegment->arr;
        }
        uint32_t *newArr = CALLOC(originalSegment->length, sizeof(uint32_t)); 
        // fprintf(stderr, "made if half way\n");
        // fprintf(stderr, "length is %d\n", originalSegment->length);
        // fprintf(stderr, "index of duplication is %d\n", index);
        for(int i = 0; i < originalSegment->length; i++){
            newArr[i] = originalSegment->arr[i];
        }
        Segment_T newSegment = initializeSegment(newArr, originalSegment->length);
        assert(newSegment->arr != NULL);
        // Segment_T oldSegment = (Segment_T) Seq_get(segments, 0);
        // Seq_put(segments, 0, newSegment);
        // fprintf(stderr,"new segment is %p\n", (void *)newSegment->arr);
        deallocateSegment((Segment_T)Seq_put(segments, 0, newSegment));

        return newSegment->arr;
        // Segment_T oldSegment = (Segment_T) Seq_put(segments, 0, newSegment);
        // fprintf(stderr, "old segment pointer is %u\n", oldSegment->arr[0]);
        // fprintf(stderr, "made it to the end of segmentDuplicate\n");
}
