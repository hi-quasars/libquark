#ifndef _QUASAR_SIGNAL_H
#define _QUASAR_SIGNAL_H

namespace quasar {
    
    //Signal operations
    //
    typedef void (*Action) (void* );      //Simple Action

    enum ActionPolicy {
        Q_SIGNORMAL,          //  SA_RESTART | SA_SIGINFO : INFO - need to get more info(such as some tracing info, file numbers, etc )
        Q_SIGOTHER            //  undefined
    };

//---------------- need to re-define in impl-----------
#define Q_SIGCHLD

#define Q_SIGPIPE
#define Q_SIGIO

#define Q_SIGINT
#define Q_SIGQUIT
#define Q_SIGTERM
//SIGKILL CANNOT BE DELIVERED

#define Q_AC_IGN
//---------------- need to re-define in impl-----------

}

#include "SignalImpl.h"

#endif
