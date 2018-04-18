#ifndef _QUASAR_THREADS_H
#define _QUASAR_THREADS_H

#include <common.h>
#include <QuasarError.h>

// 
// Multi-threading Base 
//  use: pthread / std::thread
//

namespace quasar {
    
    //a thread:
    //   @note       - default: detached, syscope 
    //   @default    - detachstate = joinable; scope = system_scope;
    //   detachstate - joinable       / detachable
    //   scope       - system_scope   / process_scope
    //
    //   state       - ref. by started & exited.
    //   
    
    // Base Declaration
    class Thread {
        public:
            typedef void *(*function)(void*);
            typedef void * func_args;

            Thread(bool is_joinable, bool is_syscope, function _fn, func_args _args);
            virtual ~Thread() { }
            static void *th_routine(void *th_args);

            // Note - 
            //  @error    - return error code style.
            //  @usage    - Thread::New(...);
            //                  ptr->Init();
            //                  ptr->Start();
            //
            static  QError New(Thread **ptr, function _fn, func_args _args);
            static  QError New(Thread **ptr, function _fn, func_args _args, bool is_joinable, bool is_syscope);
            virtual QError Init()      = 0;
            virtual QError Start()     = 0;
            virtual QError Join(void **)      = 0;

            inline void init(bool is_joinable, bool is_syscope) {
                attr1.allflags = 0;
                attr1.attr.detachstate = is_joinable ? 0 : 1;
                attr1.attr.scope = is_syscope ?  0 : 1;
            }
            inline bool is_started() {
                return attr1.attr.started == 1;
            }
            inline bool is_exited() {
                return attr1.attr.exited == 1;
            }
        protected:
            inline void set_started() {
                attr1.attr.started = 1;
            }
            inline void set_exited() {
                attr1.attr.exited = 1;
            }
            
            struct runtime {
                function    user_fn;
                func_args   user_args;
                Thread*     priv;
                runtime(function _user_fn, func_args _user_args, Thread* _priv) : 
                    user_fn(_user_fn),
                    user_args(_user_args),
                    priv(_priv) { }
            };

            union attributes{
                struct {
                    u8 detachstate : 1;    // 0 - joinable,    1 - detachable
                    u8 scope : 1;          // 0 - sys,         1 - process
                    u8 started : 1;        // 0 - not started, 1 - started
                    u8 exited  : 1;        // 0 - not exited,  1 - exited
                    u8 reserve : 1;
                } attr;
                u8 allflags;
            };

            attributes      attr1;
            runtime         rt1;
    };

} // namespace quasar

#define _Q_USE_PTHREAD
#include "ThreadsImpl.h"


#endif
