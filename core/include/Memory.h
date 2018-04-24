
//implementation of the 'mm' part of libquark

#ifndef quasar_MEMORY_H 
#define quasar_MEMORY_H


/*
 *      MemPool:
 *      alloc(N) ----> void* addr       ---> ---> XXX_Allocator::Alloc ----> XXX_MemPiece_Handler(Maybe a 64-bit int ) ---> ---> void* addr
 *      free(addr) --> null             ---> addr --> XXX_MemPiece_Handler ---> ---> XXX_Allocator::Dealloc
 *
 *
 * */


class MMNaive {

    NaivePage* PagePool;
};

class NaivePage {
    struct meta {
    
    };
};


#endif
