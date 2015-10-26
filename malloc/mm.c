/* 
 *  Explicit list allocator.  Uses a best fit algorithm to find a good match, 
 *  but stops after a certain amount of tries to maintain performance.
 *  Bryan Wade
 *      */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mm.h"
#include "memlib.h"


/* $begin mallocmacros */
/* Basic constants and macros */
#define WSIZE       4       /* Word and header/footer size (bytes) */ //line:vm:mm:beginconst
#define DSIZE       8       /* Doubleword size (bytes) */
#define CHUNKSIZE  (1<<8)  /* Extend heap by this amount (bytes) */  //line:vm:mm:endconst 

#define MAX(x, y) ((x) > (y)? (x) : (y))  

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc)) //line:vm:mm:pack

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))            //line:vm:mm:get
#define PUT(p, val)  (*(unsigned int *)(p) = (val))    //line:vm:mm:put

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)                   //line:vm:mm:getsize
#define GET_ALLOC(p) (GET(p) & 0x1)                    //line:vm:mm:getalloc

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)                      //line:vm:mm:hdrp
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) //line:vm:mm:ftrp

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE))) //line:vm:mm:nextblkp
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) //line:vm:mm:prevblkp
/* $end mallocmacros */

/* Global variables */
static void *heap_listp;  /* Pointer to first block */  
/*
#ifdef NEXT_FIT
static char *rover;           
#endif*/

static void *heap_bot;



#define SET_NEXT(bp, next)  *(unsigned int *) ((bp)+WSIZE) = (unsigned int) ((size_t) ((next) - heap_bot))
#define SET_PREV(bp, prev)  *(unsigned int *) (bp) = (unsigned int) ((size_t) ((prev) - heap_bot))

#define GET_NEXT(bp)  (void *) (*(unsigned int *) ((bp) + 4) + heap_bot)
#define GET_PREV(bp)  (void *) (*(unsigned int *) (bp) + heap_bot)



#define NEXT_CHECK(bp) (*(int *) ((bp) + 4) == -1)

#define SET_NEXT_PREV(bp) SET_NEXT(GET_PREV(bp), GET_NEXT(bp)); SET_PREV(GET_NEXT(bp), GET_PREV(bp))

#define PUT_HD_FT(bp, size, alloc)  PUT(HDRP(bp), PACK(size, alloc)); PUT(FTRP(bp), PACK(size, alloc))

#define INIT_PREV_NEXT(bp, heap_listp)  SET_PREV(bp, heap_listp); SET_PREV(GET_NEXT(heap_listp), bp); SET_NEXT(bp, GET_NEXT(heap_listp)); SET_NEXT(heap_listp, bp)

/* Function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static inline void place(void *bp, size_t asize);
static inline void *find_fit(size_t asize);
static inline void *coalesce(void *bp);
static void printblock(void *bp); 
static void checkheap(int verbose);
static void checkblock(void *bp);

/* 
 *  * mm_init - Initialize the memory manager 
 *   */
int mm_init(void) 
{
	 
   /* Create the initial empty heap */
    if ((heap_listp = mem_sbrk(5*DSIZE)) == (void *)-1) 
	return -1;

	heap_bot = mem_heap_lo();
    	PUT(heap_listp, 0);             /* Alignment padding */
    
 	/* Adds Prologue footer and header */	
	PUT_HD_FT(heap_listp + (DSIZE), 2*DSIZE, 1);

	PUT_HD_FT(heap_listp + (3*DSIZE), 2*DSIZE, 1);
	
	PUT(heap_listp + (9 *WSIZE), PACK(0,1));
 
    
    
	//initialization pointers for explicit list
	*(int *) (heap_listp + (3*DSIZE)) = -1;
	SET_NEXT(heap_listp + (3*DSIZE), heap_listp + (DSIZE));

	SET_PREV(heap_listp +(DSIZE), heap_listp + (3*DSIZE));
	*(int *) (heap_listp + (DSIZE) + WSIZE) = -1;
	
	heap_listp += (3*DSIZE);

	

    	/* Extend the empty heap*/
    
	if(extend_heap(WSIZE + 2) == NULL)
		return -1;
    	return 0;
}

/* 
 *  * mm_malloc - Allocates a block of at least the size (plus header/footers)
 *   */
void *mm_malloc(size_t size) 
{
    size_t asize;      /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    char *bp;      


    /* Ignore spurious requests */
    if (size == 0)
	return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE)   
	asize = 2*DSIZE;       
    else
	asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE); 

    /* Searches the explicit free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {  
	place(bp, asize); 
	return bp;
    }

    /* If there isn't a fit, then extend the heap */
    extendsize = MAX(asize,CHUNKSIZE);     
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)  
	return NULL;       
    place(bp, asize);   
    return bp;
} 

/* 
 *  * mm_free - Free a block, sets headers, and then coalesces
 *   */
void mm_free(void *bp)
{
    	if((bp == NULL) || ((*HDRP(bp) & 7)) != 1)
	{
		return;
	}
   	size_t size = GET_SIZE(HDRP(bp));
    
    

    	PUT_HD_FT(bp, size, 0);
	
	INIT_PREV_NEXT(bp, heap_listp);

	coalesce(bp);
}

/*
 *  * coalesce - Checks both the previous block and next block and 
 *  combines them together
 *   */
static inline void *coalesce(void *bp) 
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
	
    if (prev_alloc && next_alloc) {            /* Case 1 */
	return bp;
    }

    else if (prev_alloc && !next_alloc) {      /* Case 2 */

	char * nextB = NEXT_BLKP(bp);

	SET_NEXT_PREV(nextB);
	 
	
	size += GET_SIZE(HDRP(nextB));

	PUT_HD_FT(bp, size, 0);
    }

    else if (!prev_alloc && next_alloc) {      /* Case 3 */
	
	SET_NEXT_PREV(bp);
	
	size += GET_SIZE(HDRP(PREV_BLKP(bp)));
	
	bp = PREV_BLKP(bp);
	
	PUT_HD_FT(bp, size, 0);
	
    }

    else {                                     /* Case 4 */
	
	SET_NEXT_PREV(bp);
	
	SET_NEXT_PREV(NEXT_BLKP(bp));

	size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
	    GET_SIZE(FTRP(NEXT_BLKP(bp)));

	bp = PREV_BLKP(bp);
	
	PUT_HD_FT(bp, size, 0);
    }

    return bp;
}


/*
 *  * mm_realloc - Naive implementation of realloc
 *   */
void *mm_realloc(void *ptr, size_t size)
{
    size_t oldsize;
    void *newptr;

    /* frees if size is 0*/
    if(size == 0) {
	mm_free(ptr);
	return 0;
    }

    /* If the ptr is null then malloc a new block */
    if(ptr == NULL) {
	return mm_malloc(size);
    }

    newptr = mm_malloc(size);

    /* If it fails then dont change the old pointer  */
    if(!newptr) {
	return 0;
    }

    /* Copies data over */
    oldsize = GET_SIZE(HDRP(ptr));
    if(size < oldsize) oldsize = size;
    memcpy(newptr, ptr, oldsize);

    /* Frees block */
    mm_free(ptr);

    return newptr;
}

/* 
 *  * checkheap - We don't check anything right now. 
 *   */
void mm_checkheap(int verbose)  
{
verbose = 0; 
}

/* 
 *  * extend_heap - Extend heap with free block and return its block pointer
 *   */
static void *extend_heap(size_t words) 
{
   	void *bp;
    	size_t size;
	
	if(words < WSIZE)
	{
		words = WSIZE;
	}
	
    	/* Allocate an even number of words to maintain alignment */
   	 size = (words % 2) ? (words+1) * WSIZE : words * WSIZE; 
    	if ((long)(bp = mem_sbrk(size)) == -1)  
		return NULL; 


    	/* Initialize free block header, footer and epilogue header */
    	PUT_HD_FT(bp, size, 0);
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); 
		
	INIT_PREV_NEXT(bp, heap_listp);
    	/* Coalesce if the previous block was free */
    	return coalesce(bp); 
}

/* 
 *  * place - Adds a block of asize bytes at the beginning of free block bp 
 *   *         and splits it if there si enough space to make another block
 *    */
static inline void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));   
	 
	/*Splits block, if to big*/
	if((csize - asize) >= (2*DSIZE))
	{

		SET_NEXT_PREV(bp);
	
		PUT_HD_FT(bp, asize, 1);
		bp = NEXT_BLKP(bp);
	
		PUT_HD_FT(bp, csize - asize, 0);
	
		INIT_PREV_NEXT(bp, heap_listp);

		coalesce(bp);
	}
	else
	{
		 SET_NEXT_PREV(bp);
		
		PUT_HD_FT(bp, csize, 1);
   	}
}

/* 
 *  * find_fit - Find a fit for a block with asize bytes Uses a best fit
 *  algorithm with a maximum number of tries.
 *   */
static inline void *find_fit(size_t asize)
{


    	void * currBlock;
	void *prevFitBlock = NULL;
		
	int maxTries = 500;

	currBlock = GET_NEXT(heap_listp);
	for(int tries = 0; !NEXT_CHECK(currBlock); tries++)
	{
		/*If a block of exactly the right size exists, return that*/
		if(asize == GET_SIZE(HDRP(currBlock)))
		{
			return currBlock;
		}
		else if ((asize < GET_SIZE(HDRP(currBlock))))
		{
			prevFitBlock = currBlock;
			
		}

		/*Sets maximum amount of blocks to try*/
		if(tries >= maxTries)
		{
			return prevFitBlock;
		}
		currBlock = GET_NEXT(currBlock);
	}

	/* if no more valid blocks exist, return best current valid block */
	return prevFitBlock;
}

static void printblock(void *bp) 
{
    size_t hsize, halloc, fsize, falloc;

    checkheap(0);
    hsize = GET_SIZE(HDRP(bp));
    halloc = GET_ALLOC(HDRP(bp));  
    fsize = GET_SIZE(FTRP(bp));
    falloc = GET_ALLOC(FTRP(bp));  

    if (hsize == 0) {
	printf("%p: EOL\n", bp);
	return;
    }
}

static void checkblock(void *bp) 
{
    if ((size_t)bp % 8)
	printf("Error: %p is not doubleword aligned\n", bp);
    if (GET(HDRP(bp)) != GET(FTRP(bp)))
	printf("Error: header does not match footer\n");
}

/* 
 *  * checkheap - Minimal check of the heap for consistency 
 *   */
void checkheap(int verbose) 
{
    char *bp = heap_listp;

    if (verbose)
	printf("Heap (%p):\n", heap_listp);

    if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || !GET_ALLOC(HDRP(heap_listp)))
	printf("Bad prologue header\n");
    checkblock(heap_listp);

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
	if (verbose) 
	    printblock(bp);
	checkblock(bp);
    }

    if (verbose)
	printblock(bp);
    if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
	printf("Bad epilogue header\n");
}


