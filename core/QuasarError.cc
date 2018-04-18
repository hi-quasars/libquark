#include <common.h>
#include <QuasarError.h>

namespace quasar {
    static const char *gErrMsg[] = {
            "OK",
            "QThreadNotJoinable"
        };
    static char gErrMsgBuf[TBUFSIZE];

    char *QError::msgbuf = gErrMsgBuf;
    std::string QError::ToString () {
        switch(err){
        case QSError::OK:
            return gErrMsg[err];
        case QSError::QThreadNotJoinable:
            return gErrMsg[err - QSError::QThreadNotJoinable + 1];
        }
        sprintf(msgbuf, "%d", err);
        return std::string("Unknown: ") + msgbuf;
    }


}  // namespace quasar
