#ifndef Q_BITMAP_H
#define Q_BITMAP_H

#include <stdint.h>


typedef uint8_t byte_t;

struct bitmap_s {
    int used;
    int max;
    
    byte_t *ctn;
    int     len;
};
typedef struct bitmap_s bitmap_t;

/*
 * alloc mem internal
 * */
bitmap_t* alloc_bitmap(int max);
int dealloc_bitmap(bitmap_t* bm);

int alloc_bit(bitmap_t *bm);
void release_bit(bitmap_t *bm);



#endif
