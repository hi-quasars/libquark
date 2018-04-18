#include <string>
#include <typeinfo>
#include "../../include/common.h"

using namespace quasar;

int test1()
{
    std::string str1 = "hahaha";
    int flag = 0;
    printf("%s\n", CSTR1(str1));
    return 0;
}

// 
typedef int (*FUNC) ();
int test_x() {
    return 1;
}

int test2()
{
    FUNC fp = test_x;
    // TYPE_CHECK(test_x, FUNC);  <------- Check fail
    TYPE_CHECK(&test_x, FUNC); //<------- Check pass

    printf("%d\n", fp());
    return 1;
}

#include "temp.h"

int test3()
{
    Log1();
}



#ifdef __GNUC__
#define ISGNU "Yes"
#else
#define ISGNU "No"
#endif

#include <pthread.h>
#include <cerrno>  //errno
#include <cstdlib> //EXIT_FAILURE
#include <cassert> //assert

#define error_sys(en, msg) \
    do {  errno = en, perror(msg); exit(EXIT_FAILURE); } while(0)

#define EXIT_IF_ERR(ret, func) \
    do { if (0 != ret) { error_sys(ret, func); } } while(0)

namespace PthreadAttr {
    const int get = 0;
    const int set = 1;

    enum {
        int_attr_start,
        detachstate = 0,            // int
        scope,                      // 1
        schedpolicy,                // 1
        inheritsched,
        int_attr_end,               //
        sz_attr_start,
        guardsize,                  //
        sz_attr_end,
        sp_start,
        schedparam,
        schedpri = schedparam,
        sp_end,
        stk_start,
        stack,
        stack_addr = stack,
        stack_size = stack,
        stk_end
    };
}

#define CONCAT(A, B) A##B
#define PTHREAD_GET_ATTR(AttrName) CONCAT(pthread_attr_get, AttrName)
#define PTHREAD_SET_ATTR(AttrName) CONCAT(pthread_attr_set, AttrName)
#define PTHATTR(A) PthreadAttr::A

template<typename RefType>
RefType* PtrConvert(void* x) {
    return reinterpret_cast<RefType*>(x);
}
template<typename RefType>
RefType DeRef(void *x) {
    return *PtrConvert<RefType>(x);
}


#define DEALWITH_PTHREAD_INT(AttrName, GetSet, PAPtr, VarPtr)  do { \
        if ( PTHATTR(get) == GetSet) {                              \
            PTHREAD_GET_ATTR(AttrName) (PAPtr, PtrConvert<int>(VarPtr)) ;    \
        } else if ( PTHATTR(set) == GetSet ) {              \
            PTHREAD_SET_ATTR(AttrName) (PAPtr, DeRef<int>(VarPtr) );   \
        } else {    \
            ;       \
        }           \
    }while(0)
#define DEALWITH_PTHREAD_SZ(AttrName, GetSet, PAPtr, VarPtr)  do { \
        if ( PTHATTR(get) == GetSet) {                              \
            PTHREAD_GET_ATTR(AttrName) (PAPtr, PtrConvert<size_t>(VarPtr)) ;    \
        } else if ( PTHATTR(set) == GetSet ) {              \
            PTHREAD_SET_ATTR(AttrName) (PAPtr, DeRef<size_t>(VarPtr) );   \
        } else {    \
            ;       \
        }           \
    }while(0)
#define DEALWITH_PTHREAD_SCHEDPARAM(GetSet, PAPtr, SPPtr) do { \
        if ( PTHATTR(get) == GetSet) { \
            PTHREAD_GET_ATTR(schedparam) (PAPtr, PtrConvert<struct sched_param>(SPPtr)); \
        } else if ( PTHATTR(set) == GetSet ) { \
            PTHREAD_SET_ATTR(schedparam) (PAPtr, PtrConvert<const struct sched_param>(SPPtr));  \
        } else {    \
            ;       \
        }           \
    }while(0)
#define DEALWITH_PTHREAD_STACK(GetSet, PAPtr, STKADDRPtr, STKSIZEPtr) do { \
        if ( PTHATTR(get) == GetSet) {                                  \
            PTHREAD_GET_ATTR(stack) (PAPtr, STKADDRPtr, STKSIZEPtr);    \
        } else if ( PTHATTR(set) == GetSet ) {                          \
            PTHREAD_SET_ATTR(stack) (PAPtr, *STKADDRPtr, *STKSIZEPtr);  \
        } else {    \
            ;       \
        }           \
    }while(0)

class thread_attr_for_pthread {
public:
    thread_attr_for_pthread( pthread_attr_t pa) : _p_attr(pa) { }
    std::string CheckAttribute(int which) {
        GetAttr(which);
        switch(which) {
        case PTHATTR(detachstate) :
            return EQ(int_attr, PTHREAD_CREATE_DETACHED, PTHREAD_CREATE_JOINABLE);
        case PTHATTR(scope) :
            return EQ(int_attr, PTHREAD_SCOPE_SYSTEM, PTHREAD_SCOPE_PROCESS);
        case PTHATTR(schedpolicy) :
            return EQ(int_attr, SCHED_OTHER, SCHED_FIFO, SCHED_RR);
        case PTHATTR(inheritsched):
            return EQ(int_attr, PTHREAD_INHERIT_SCHED, PTHREAD_EXPLICIT_SCHED);
        }
        return std::string("Not Support!");
    }

private:

    void _DoWithAttr(int what, int way, pthread_attr_t *pa, void *var) {
        assert(what >= PTHATTR(detachstate) && what <= PTHATTR(stack_size));
        switch(what) {
        case PTHATTR(detachstate) :
            DEALWITH_PTHREAD_INT(detachstate, way, pa, var); 
            break;
        case PTHATTR(scope) :
            DEALWITH_PTHREAD_INT(scope, way, pa, var); 
            break;
        case PTHATTR(schedpolicy) :
            DEALWITH_PTHREAD_INT(schedpolicy, way, pa, var); 
            break;
        case PTHATTR(schedparam) :
            DEALWITH_PTHREAD_SCHEDPARAM(way, pa, var); 
            break;
        case PTHATTR(inheritsched):
            DEALWITH_PTHREAD_INT(inheritsched, way, pa, var);
            break;
        case PTHATTR(guardsize) :
            DEALWITH_PTHREAD_SZ(guardsize, way, pa, var); 
            break;
        case PTHATTR(stack) :
            struct stack_attr *stkp = reinterpret_cast<struct stack_attr *>(var);
            DEALWITH_PTHREAD_STACK(way, pa, &(stkp->addr), &(stkp->stack_size)); 
            break;
        };
    }
    void GetAttr(int what) {
        void *ret;
        if (RANGE(what, PTHATTR(int_attr_start), PTHATTR(int_attr_end))) {
            ret = reinterpret_cast<void*>(&int_attr);
        } else if (RANGE(what, PTHATTR(sz_attr_start), PTHATTR(sz_attr_end))) {
            ret = reinterpret_cast<void*>(&sz_attr);
        } else if (RANGE(what, PTHATTR(sp_start), PTHATTR(sp_end))) {
            ret = reinterpret_cast<void*>(&sp);
        } else if (RANGE(what, PTHATTR(stk_start), PTHATTR(stk_end))) {
            ret = reinterpret_cast<void*>(&stk);
        } else {
            return;
        }
        _DoWithAttr( what, PTHATTR(get), &_p_attr, ret);
    }
    int SetAttr(int what) {
    }
    
    pthread_attr_t _p_attr;
    
    struct stack_attr {
        void    *addr;
        size_t  stack_size;
        stack_attr() { }
        stack_attr(void *_a, size_t _s) : addr(_a), stack_size(_s) { }
    };
    
    struct stack_attr   stk;
    struct sched_param  sp;
    int int_attr;
    size_t sz_attr;
};


// demo for pthread.
void display_pthread_attr(pthread_attr_t *attrptr) {
    int ret, detach, scope, inheritsched, schedplc;
    size_t guard_size, stack_size;
    void * stack_addr;
    struct sched_param sp;
    ret = pthread_attr_getdetachstate(attrptr, &detach);
    EXIT_IF_ERR(ret, "pthread_attr_getdetachstate");
    
    ret = pthread_attr_getscope(attrptr, &scope);
    EXIT_IF_ERR(ret, "pthread_attr_getscope");
    
    ret = pthread_attr_getschedpolicy(attrptr, &schedplc);
    EXIT_IF_ERR(ret, "pthread_attr_getschedpolicy");

    ret = pthread_attr_getinheritsched(attrptr, &inheritsched);
    EXIT_IF_ERR(ret, "pthread_attr_getinheritsched");

    ret = pthread_attr_getschedparam(attrptr, &sp);
    EXIT_IF_ERR(ret, "pthread_attr_getschedparam");

    ret = pthread_attr_getguardsize(attrptr, &guard_size);
    EXIT_IF_ERR(ret, "pthread_attr_getguardsize");

    ret = pthread_attr_getstack(attrptr, &stack_addr, &stack_size);
    EXIT_IF_ERR(ret, "pthread_attr_getstack");
    
}


//EQ Macro Override
int test4()
{
    const char *s1 = "bbb", *s2 = "bbb";
    if (STREQ(s1, s2)) {
        printf("eq!\n");
    }

    enum {
        PS1,
        PSS,
        POK
    };
    int i = PSS;  
    printf("%s\n" , EQ(i, PS1, PSS, POK) );
    return 0;
}

int test5()
{
    pthread_attr_t pattr;
    int ret = pthread_attr_init(&pattr);
    EXIT_IF_ERR(ret, "pthread_attr_init");
    
    thread_attr_for_pthread ta(pattr);
    printf("%s\n",ta.CheckAttribute(PTHATTR(detachstate)).c_str());
    printf("%s\n",ta.CheckAttribute(PTHATTR(scope)).c_str());
    printf("%s\n",ta.CheckAttribute(PTHATTR(schedpolicy)).c_str());
    printf("%s\n",ta.CheckAttribute(PTHATTR(inheritsched)).c_str());
}

union Attr{                                                      
    struct {                                                     
        u8 detachstate : 1;    // 0 - joinable,    1 - detachable
        u8 scope : 1;          // 0 - sys,         1 - process   
        u8 reserve : 1;                                          
    // attr(bool is_joinable, bool is_syscope) :                 
    //    detachstate(is_joinable ? 0 : 1),                      
    //    scope(is_syscope ? 0 : 1) { }                          
    } _attr;                                                     
    u8 allflags;                                                 
};                                                               
int test6()
{
	Attr x1;
	x1._attr.scope = 1;
	printf("%d\n", x1._attr.detachstate);
	printf("%d\n", x1._attr.scope);

    printf("%d\n", x1.allflags);

	x1.allflags = 0;
	printf("%d\n", x1._attr.detachstate);

}


int
main()
{
    test6();
    return 0;
}
