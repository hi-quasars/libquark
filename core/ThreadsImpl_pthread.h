#ifndef _QUASAR_THREADSIMPL_PTHREAD_H
#define _QUASAR_THREADSIMPL_PTHREAD_H
namespace quasar {
    class Th_pthread : public Thread {
    public:
        Th_pthread(function _fn, func_args _args);
        Th_pthread(function _fn, func_args _args, bool is_joinable, bool is_syscope);

        virtual QError Init();
        virtual QError Start();
        virtual QError Join(void **);

    private:
        pthread_attr_t  pa;
        pthread_t       tid;
    };
} // namespace quasar 

#endif //#ifndef _QUASAR_THREADSIMPL_PTHREAD_H
