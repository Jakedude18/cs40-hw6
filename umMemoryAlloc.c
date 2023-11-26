/**
 * umMemoryAlloc.c
 * Authors: Jake Kerrigan, Jacob Frieman
 * November 13, 2023
 * 
 * Handles deallocation and allocation of dynamic memory. Called by the 
 * umCommands submodule for the map segment, unmap segment, load program,
 * and load value commands.
 */
#include "umMemoryAlloc.h"
#include <seq.h>
#include <stdio.h>


/**********Memory********
* holds a sequence of segments that represents memory for the um, and a
* sequence of recycled identifiers for future segment mapping.
************************/
struct Memory {
        Seq_T segments;
        /* unmapped segment ids */
        Seq_T ids;             
};


/**********Segment********
* holds an array of uint32_ts which represent words in a segment and the
* length of this array.
************************/
struct Segment {
        uint32_t * arr;
        int length;
};

typedef struct Segment *Segment_T;

/**********initializeSegment********
* allocates memory for a new segment and initializes it.
* Inputs:
*       uint32_t *arr: array to be pointed to by the new segment.
*       int length: integer that holds the length of the array.
* Returns:
*       Segment_T of the new segment.
* Notes: segment to be deallocated in deallocateSegment
************************/
Segment_T initializeSegment(uint32_t *arr, int length)
{
        Segment_T segment= ALLOC(sizeof(struct Segment));
        segment->arr = arr;
        segment->length = length;
        return segment;
}

/**********deallocateSegment********
* frees the memory of a segment
* Inputs:
*       Segment_T segment: segment to be freed
* Returns:
*       void
************************/
void deallocateSegment(Segment_T segment)
{
    FREE(segment->arr);
    FREE(segment);
}




/**********deallocateSegment********
* frees the memory of a segment
* Inputs:
*       Segment_T segment: segment to be freed
* Returns:
*       void
************************/
Memory_T initalizeMemory(uint32_t * initial, int length)
{
        Memory_T memory = ALLOC(sizeof(struct Memory));
        memory->segments = Seq_new(1);
        memory->ids = Seq_new(0);
        Segment_T firstSegment = initializeSegment(initial, length);
        Seq_T segments = memory->segments;
        Seq_addlo(segments, firstSegment);
        return memory;
}


/**********deallocateMemory********
* frees a memory struct
* Inputs:
*       Memory_T memory: memory to be freed
* Returns:
*       void
************************/
void deallocateMemory(Memory_T memory)
{
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

/**********loadMemory********
* maps and allocates memory for a new segment in memory. If there are recycled
* identifiers to be used for this new segment, the segment in that identifier
* is deallocated and the new segment is stored there.
* Inputs:
*       uint32_t length: holds the length of the segment to be mapped.
*       Memory_T memory: holds memory for um.
* Returns:
*       uint32_t of the index where the segment was mapped in memory.  
* Notes: new segment can either be deallocated in this function or in 
* deallocateSegment.
************************/
uint32_t loadMemory(uint32_t length, Memory_T memory)
{
        Seq_T segments = memory->segments;
        uint32_t * newArr = CALLOC(length, sizeof(uint32_t));
        Segment_T newSeg = initializeSegment(newArr, length);
        /* figure out 32 bit identifier (index) */
        uint32_t index = Seq_length(segments);
        if(Seq_length(memory->ids) > 0) {
                index = (uint32_t)(uintptr_t) Seq_remlo(memory->ids);
                Segment_T oldVal = (Segment_T) 
                        Seq_put(segments, index, newSeg);
                if(oldVal != NULL) {
                        deallocateSegment(oldVal);
                }
        }
        else {
                Seq_addhi(segments, newSeg);
        }
        return index;
}

/**********unloadMemory********
* unmaps a segment in memory by adding its index to the sequence of recycled
* identifiers.
* Inputs:
*       uint32_t index: holds the index of segment to be unmapped.
*       Memory_T memory: holds memory for um.
* Returns:
*       void  
************************/
void unloadMemory(uint32_t index, Memory_T memory)
{
        uint32_t id = index;
        Seq_addlo(memory->ids, (void *)(uintptr_t)id);

}

/**********getMemory********
* gets the value stored in memory from the given index and offset
* Inputs:
*       uint32_t index: index of value to be retreived.
*       uint32_t offset: offset of value to be retreived.
*       Memory_T memory: holds memory for um.
* Returns:
*       uint32_t of the value stored in $m[index][offset]  
************************/
uint32_t getMemory(uint32_t index, uint32_t offset, Memory_T memory)
{
        Seq_T segments = memory->segments;
        Segment_T segment = (Segment_T) (Seq_get(segments, index));
        assert(segment->arr != NULL);
        return segment->arr[offset];
}

/**********setMemory********
* sets a given value to memory in the given index and offset
* Inputs:
*       uint32_t index: index in memory to set the value to.
*       uint32_t offset: offset in memory to set the value to.
*       uint32_t value: value to be set in $m[index][offset]
*       Memory_T memory: holds memory for um.
* Returns:
*       void  
************************/
void setMemory(uint32_t index, uint32_t offset,
        uint32_t value, Memory_T memory)
{
        
        Seq_T segments = memory->segments;
        Segment_T segment = (Segment_T) (Seq_get(segments, index));
        assert(segment->arr != NULL);
        segment->arr[offset] = value;
}

/**********segmentDuplicate********
* sets program instructions stored in $m[0] to program instruction held in 
* $m[index]. deallocates segment of old program.
* Inputs:
*       uint32_t index: index in memory that holds the new program.
*       Memory_T memory: holds memory for um.
* Returns:
*       pointer to new program
* Notes: does nothing if index is 0. new program is deallocated in this
* function or in deallocateSegment.
************************/
uint32_t * segmentDuplicate(int index, Memory_T memory)
{
        Seq_T segments = memory->segments;
        
        Segment_T originalSegment = (Segment_T) (Seq_get(segments, index));
        if(index == 0) {
            return originalSegment->arr;
        }
        uint32_t *newArr = CALLOC(originalSegment->length, sizeof(uint32_t)); 
        for(int i = 0; i < originalSegment->length; i++) {
            newArr[i] = originalSegment->arr[i];
        }
        Segment_T newSegment = 
                initializeSegment(newArr, originalSegment->length);
        assert(newSegment->arr != NULL);
        deallocateSegment((Segment_T)Seq_put(segments, 0, newSegment));

        return newSegment->arr;
}
