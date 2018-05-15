#ifndef BIT_H
#define BIT_H

// Compatieble with GNUC
//
#define MaxValOfType(Type)  ({     \
    Type __tmp = 0;       \
    ~__tmp;                        \
    })

#define MaxValOfVarType(Var) ({ \
    __typeof(Var) __tmp = 0;    \
    ~__tmp;                 \
    })

#define First0to1(Var) ({ \
    __typeof(Var) __tmp = (Var | (Var + 1)); \
    __tmp; \
    })

#define First1to0(Var) ({ \
    __typeof(Var) __tmp = (Var & (Var - 1)); \
    __tmp; \
    })

/*
 * gaudent algorithm, like binary search.
 * param - @Varx
 * only 1 bit of @x is 1. return the index of this bit.
 * e.g.
 *  0x0400 --> 10
 * */
#define NTZ8Bit(Varx) ({            \
    int __b0,__b1,__b2;             \
    __b0 = (Varx & 0x0f) ? 0 : 4;   \
    __b1 = (Varx & 0x33) ? 0 : 2;   \
    __b2 = (Varx & 0x55) ? 0 : 1;   \
    __b0 + __b1 + __b2;             \
    })

#define NTZ16Bit(Varx) ({           \
    int __b0, __b1, __b2, __b3;     \
    __b0 = (Varx & 0x00ff) ? 0 : 8;    \
    __b1 = (Varx & 0x0f0f) ? 0 : 4;    \
    __b2 = (Varx & 0x3333) ? 0 : 2;    \
    __b3 = (Varx & 0x5555) ? 0 : 1;    \
    __b0 + __b1 + __b2 + __b3;         \
    })

#define NTZ32Bit(Varx) ({ \
    int __b0, __b1, __b2, __b3, __b4; \
    __b0 = (Varx & 0x0000ffff) ? 0 : 16;   \
    __b1 = (Varx & 0x00ff00ff) ? 0 : 8;    \
    __b2 = (Varx & 0x0f0f0f0f) ? 0 : 4;    \
    __b3 = (Varx & 0x33333333) ? 0 : 2;    \
    __b4 = (Varx & 0x55555555) ? 0 : 1;    \
    __b0 + __b1 + __b2 + __b3 + __b4;  \
    })
#endif
