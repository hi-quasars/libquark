#ifndef _QUASAR_SIGNAL_IMPL_H
#define _QUASAR_SIGNAL_IMPL_H

namespace quasar {

#if defined(__linux__)
#define __SIGLINUX
#else
//define other platform
#endif

//common part	
class NOLOG;
#include <log.h>



// signal operations at linux
#ifdef __SIGLINUX
#include <signal.h>


#ifdef Q_SIGCHLD
#undef Q_SIGCHLD
#define Q_SIGCHLD	SIGCHLD
#endif

#ifdef Q_SIGPIPE
#undef Q_SIGPIPE 
#define Q_SIGPIPE	SIGPIPE
#endif

#ifdef Q_SIGIO  
#undef Q_SIGIO  
#define Q_SIGIO		SIGIO
#endif

#ifdef Q_SIGINT  
#undef Q_SIGINT  
#define Q_SIGINT	SIGINT
#endif

#ifdef Q_SIGQUIT 
#undef Q_SIGQUIT 
#define Q_SIGQUIT	SIGQUIT
#endif

#ifdef Q_SIGTERM
#undef Q_SIGTERM
#define Q_SIGTERM	SIGTERM
#endif

#ifdef Q_AC_IGN
#undef Q_AC_IGN
#define Q_AC_IGN	SIG_IGN
#endif

//a Helper class only

typedef int SigType;

template<typename LogType = NoLog>
class Signal {
public:
    static QError SetActionSimple(SigType sig) { }
    static QError SetActionInfo(SigType sig) { }
private:
    static QError setaction(SigType sig, void (*cbptr)(SigType, siginfo_t *, void*));
	static void sa_sigaction_simple (SigType sig, siginfo_t *si, void *uc);     // si for `signal information`; uc for `user context`
	static void sa_sigaction_info (SigType sig, siginfo_t *si, void *uc);       // si for `signal information`; uc for `user context`
    static std::string sigstr(SigType sig) {
        const char *ret;
        switch (sig) {
            case Q_SIGCHLD:
                ret = "SIGCHLD";
                break;
            case Q_SIGPIPE:
                ret = "SIGPIPE";
                break;
            case Q_SIGIO:
                ret = "SIGIO";
                break;
            case Q_SIGINT:
                ret = "SIGINT";
                break;
            case Q_SIGQUIT:
                ret = "SIGQUIT";
                break;
            case Q_SIGTERM:
                ret = "SIGTERM";
                break;
        }
        return ret;
    }

    static struct sigaction sa_simple_;
    static struct sigaction sa_info_;
    static LogType* l_;
};

template<typename LogType = NoLog>
QError Signal<LogType>::setaction(SigType sig, void (*cbptr)(SigType, siginfo_t *, void*)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = cbptr;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;

    if (sigaction(sig, &sa, NULL) < 0) {
        return QError(errno);
    }
    return QError();
}

template<typename LogType = NoLog>
QError Signal<LogType>::SetActionSimple(SigType sig) { 
    return setaction(sig, &sa_sigaction_simple);
}

template<typename LogType = NoLog>
QError Signal<LogType>::SetActionInfo(SigType sig) { 
    return setaction(sig, &sa_sigaction_info);
}

template<typename LogType = NoLog>
void Signal<LogType>::sa_sigaction_simple(SigType sig, siginfo_t *si, void *uc) {
    l_->log("Signal [ %s ] is Caught", sigstr(sig));
}

template<typename LogType = NoLog>
void Signal<LogType>::sa_sigaction_info(SigType sig, siginfo_t *si, void *uc) {
    l_->log("Signal [ %s ] is Caught", sigstr(sig));
    l_->log("\tsignal number: si_signo=%d\n", si->si_signo);
    l_->log("\terror number: si_errno=%d\n", si->si_errno);
    l_->log("\tsignal code: si_code=%d\n", si->si_code);
    l_->log("\tsending process ID: si_pid=%d\n", si->si_pid);
    l_->log("\tsending process user's ID: si_uid=%d\n", si->si_uid);
    l_->log("\texit value or signal: si_status=%d\n", si->si_status);
    l_->log("\tband event for SIGPOLL: si_band=%ld\n", si->si_band);
}

#else
//other platform



#endif

}   // namespace quasar
#endif
