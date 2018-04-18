#include <Threads.h>
namespace quasar {

//common part
Thread::Thread(bool is_joinable, 
    bool is_syscope, 
    function _fn, 
    func_args _args) : 
    rt1(_fn, _args, this) {
        init(is_joinable, is_syscope);
    }

void *Thread::th_routine(void *th_args) {
    void *ret;
    struct runtime* rtx = reinterpret_cast<struct runtime *>(th_args);
    rtx->priv->set_started();
    ret = (*rtx->user_fn)(rtx->user_args);
    rtx->priv->set_exited();
    return ret;
}

namespace {
    
    enum {
        DetachState = 0,
        Scope,
        StateStarted,
        StateExited,
    };

#define ATTR_IS_JOINABLE(U8)    (0 == U8BIT(U8, DetachState))
#define ATTR_IS_SYSCOPE(U8)     (0 == U8BIT(U8, Scope))

}

//common part

#ifdef _Q_USE_PTHREAD
//pthread implementations

QError Thread::New(Thread **ptr, function _fn, func_args _args) {
    int rc = 0;
    *ptr = new Th_pthread(_fn, _args);
    return rc;
}

QError Thread::New(Thread **ptr, function _fn, func_args _args, bool is_joinable, bool is_syscope) {
    int rc = 0;
    *ptr = new Th_pthread(_fn, _args, is_joinable, is_syscope);
    return rc;
}


Th_pthread::
Th_pthread(function _fn, func_args _args) : 
    Thread(false, true, _fn, _args) { 
    
    }

Th_pthread::
Th_pthread(function _fn, func_args _args, bool is_joinable, bool is_syscope) : 
    Thread(is_joinable, is_syscope, _fn, _args) { 

    }

QError Th_pthread::Init() {
    int i, rc;
    rc = pthread_attr_init(&pa);
    if (0 != rc) {
        goto ERR;
    }
    //detachstate
    i = ATTR_IS_JOINABLE(attr1.allflags) ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED;
    rc = pthread_attr_setdetachstate(&pa, i);
    if (0 != rc) {
         goto ERR;
    }
    
    //scope
    i = ATTR_IS_SYSCOPE(attr1.allflags) ? PTHREAD_SCOPE_SYSTEM : PTHREAD_SCOPE_PROCESS;
    rc = pthread_attr_setscope(&pa, i);
    if (0 != rc) {
        goto ERR;
    }

ERR:
    return rc;
}

QError Th_pthread::Start() {
    int rc = pthread_create( &tid, &pa, &Th_pthread::th_routine, reinterpret_cast<void*>(&(this->rt1)) );
    return rc;
}

QError Th_pthread::Join(void **ret) {
    if( ATTR_IS_JOINABLE(attr1.allflags) ) {
        int rc = pthread_join(tid, ret);
        return rc;
    }
    return QSError::QThreadNotJoinable;
}


#else
//...


#endif


} // namespace quasar
