#include <Bitmap.h>
#include <Bit.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>


// [0, max]
// -------- byte_t's bit num = 8 ------
// index    
// 0-7      i=0, idx = 0-7
// 8-15     i=1, idx = 0-7
//  
// index <--  i * 8 + idx
// index -->  i = index / 8,  idx = index % 8
//
bitmap_t* alloc_bitmap(int max) {
    int bytes = sizeof(byte_t) == 1 ? 
        ((max >> 3) + 1) : ((max / sizeof(byte_t)) + 1);
    bitmap_t *bptr = (bitmap_t *)malloc(sizeof(bitmap_t));
    memset(bptr, 0, sizeof(bitmap_t));
    bptr->ctn = (byte_t *)malloc(bytes * sizeof(byte_t));
    if (bptr->ctn) {
        goto ERR;
    }
    
    bptr->max = max;
    bptr->len = bytes;

    return bptr;
ERR:
    if (bptr) {
        free(bptr);
    }
    return NULL;
}

int dealloc_bitmap(bitmap_t* bm) {
    if (bm->ctn) {
        free(bm->ctn);
    }
    if (bm) {
        free(bm);
    }
}

int find_empty_slot(byte_t x) {
    byte_t f0t1 = First0to1(x);
    int zindex;
    if(sizeof(x) == 8) {
        zindex = NTZ8Bit(f0t1 ^ x);
    }else if(sizeof(x) == 16) {
        zindex = NTZ16Bit(f0t1 ^ x);
    }else if(sizeof(x) == 32) {
        zindex = NTZ32Bit(f0t1 ^ x);
    }else {
        zindex = -1;
    }
    return zindex;
}

int i_idx2index(int i, int idx) {
    return (i * (sizeof(byte_t) << 3)) + idx;
}

int index2i(int index) {
    return (index / sizeof(byte_t));
}
//offset inside the byte
int index2idx(int index) {
    return (index % sizeof(byte_t));
}


int alloc_bit(bitmap_t *bm) {
    int i, idx;

    for (i = 0; i < bm->len; i++) {
        if (bm->ctn[i] < MaxValOfType(byte_t)) {
            idx = find_empty_slot(bm->ctn[i]);
            assert(idx > 0);
            
            // idx
            // Lock ?
            // Lock-Less ?
            IndexXSet1(bm->ctn[i], idx);
            
            return (i * sizeof(byte_t) + idx);
        }
    }
    return -1;
}

// assume - Not possiable alloc the same bit twice.
//
void release_bit(bitmap_t *bm, int index) {
    int i = index2i(index);
    int idx = index2idx(index);
    
    //Lock ?
    //Lock-less ?
    //
    IndexXSet0(bm->ctn[i], idx);
}
