#ifndef _QUASAR_ERROR_H
#define _QUASAR_ERROR_H

#include <string>
#include <cerrno>

namespace quasar {

    namespace QSError{
        typedef enum {
            OK = 0,
            QThreadNotJoinable = 1000
        } ErrorCode;
    }

    //need to add EFAULT EINVAL ....

    class QError {
    public:
        QError() : err(QSError::OK) {}
        QError(int _err) : err(_err) {}
        bool isOK() { return QSError::OK == err; }
        std::string ToString();
        static const char * SysError() { return stderror(errno); }

    private:
        static char     *msgbuf;
        int             err;
    };


}
#endif
