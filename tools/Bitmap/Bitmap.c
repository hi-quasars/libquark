#include <Bitmap.h>
#include <Bit.h>
#include <malloc.h>
#include <string.h>

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

int alloc_bit(bitmap_t *bm) {
    int ret = -1, i;
    for (i = 0; i < bm->len; i++) {
        if (bm->ctn[i] < MaxValOfType(byte_t)) {
            find_empty_slot(bm->)
        }
    }

    return ret;
FULL:
    return -1;
}

void release_bit(bitmap_t *bm, int bi) {
}
