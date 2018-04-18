#ifndef quasar_COMMON_H
#define quasar_COMMON_H

#include <cstdio>
#include <ctime>
#include <string>
#include <cstring>
#include <typeinfo>

#include <stdint.h>

namespace quasar {
    typedef uint8_t     u8;
    typedef uint16_t    u16;
    typedef uint32_t    u32;
    typedef uint64_t    u64;

#define TBUFSIZE    64
#define NULLSTR     ""
    void check_cpp_version();
    
#define RESLV_USEC      0
#define RESLV_MILISEC   1

//#ifdef __GUNC__
#define TYPE_CHECK(var, type) ({  \
        __typeof(var) * __tmp;    \
        __tmp = (type *) NULL;    \
    })
//#else
//#define TYPE_CHECK(var, type)
//#endif

#define CSTR1(str) ({                   \
        TYPE_CHECK(str, std::string);   \
        str.c_str();                    \
    })

#define CSTR2C(STR) #STR

#define STREQ(str1, str2) ({      \
    std::string _tmp1 = std::string(str1);  \
    0 == _tmp1.compare(str2);               \
    })

#define NON_EQ "NONE?"
#define EQ1(i, ID1) (i == ID1 ? #ID1 : NON_EQ)
#define EQ2(i, ID1, ID2) ({                         \
        const char* _tmp = EQ1(i, ID1);             \
        STREQ(_tmp, NON_EQ) ? EQ1(i, ID2) : _tmp;   \
    })

#define EQ3(i, ID1, ID2, ID3) ({                    \
        const char* _tmp = EQ2(i, ID1, ID2);        \
        STREQ(_tmp, NON_EQ) ? EQ1(i, ID3) : _tmp;   \
    })

#define EQ_OVERRIDE(_1, _2, _3, _4, EQWHICH, ...) EQWHICH
#define EQ(...) EQ_OVERRIDE(__VA_ARGS__, EQ3, EQ2, EQ1)(__VA_ARGS__)

#define RANGE(var, LEFT, RIGHT) (var > LEFT && var < RIGHT)

#define RUN_FLAG(x)  do {  \
    printf("Func %s, %s\n", __FUNCTION__ , #x);   \
}while(0)

#define U8BIT(u8var, x) (u8var & (1 << x))

#define US_PER_SEC 1000000
#define MS_PER_SEC 1000

    //time
    inline u64 _now_timespec(int resolv) {
        struct timespec ts;
        u64 ret = 0;
        if( 0 == clock_gettime(CLOCK_REALTIME, &ts) ) {
            u64 s_rslv;
            u64 ns_rslv;
            switch (resolv) {
                case RESLV_USEC:
                    s_rslv  = 1000 * 1000;
                    ns_rslv = 1000;
                    break;
                case RESLV_MILISEC:
                    s_rslv  = 1000;
                    ns_rslv = 1000 * 1000;
                    break;
                default :
                    return ret;
            }
            ret += ts.tv_sec * s_rslv + ts.tv_nsec * ns_rslv;
        }
        return ret;
    }

    inline u64 now_in_usec() {
        return _now_timespec(RESLV_USEC);
    }
    
    inline u64 now_in_milisec() {
        return _now_timespec(RESLV_MILISEC);
    }

    std::string now_in_string();
    
    void mssleep(int );
    void ussleep(int );
    void ssleep(int );

} // namespace quasar

#endif
