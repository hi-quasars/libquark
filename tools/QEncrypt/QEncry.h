
#ifndef QENCRY_H
#define QENCRY_H

#include <cstdio>
#include <stdint.h>

//3rdparty
#include "chacha20poly1305/chacha.h"
#include "chacha20poly1305/chachapoly_aead.h"
#include "chacha20poly1305/poly1305.h"

#define DEBUG


namespace Quark {

typedef uint8_t uchar_t;

const size_t QEBlockSize = 512;
const size_t QEKeySize = 32;
const uchar_t *QEKeys = (uchar_t*)"879b976f9e1d328865559a771b982120";


class QEncryObject {
public:
    enum {
      rOK = 0,
      rEOF,
      rERR,
      rENVAL
    };

    QEncryObject(const char *filename);
    ~QEncryObject();

    int ReadBlock(void *buf);       // Read a block and decrypt
    int WriteBlock(void *buf, size_t len);      // encript and write a line

    void QkEncript(uchar_t *from, size_t len);
    void QkDecript(uchar_t *from, size_t len);
#ifdef DEBUG    
    void PrintUchar(uchar_t *ptr, size_t len);
#endif
    uchar_t* InternalBuffer(int type) {
        if (0 == type) {
            return read_buffer;
        } else if (1 == type) {
            return write_buffer;
        } else if (2 == type) {
            return encrypt_buffer;
        }
    }
    size_t InternalBSize(int type) {
        if (0 == type) {
            return rb_sz;
        } else if (1 == type) {
            return wb_sz;
        } else if (2 == type) {
            return eb_sz;
        }
    }

private:
    int internal_read_rbuffer();
    int internal_write_wbuffer();
    void internal_encript(uchar_t *from, size_t len, int en_or_de);
    //int internal_read_file(uchar_t* buf, size_t sz);
    
    // encription materials
    struct chachapolyaead_ctx   encr_adea_ctx;
    const uchar_t *             encr_key;
    size_t                      encr_key_len;


    // other meta
    int state;
    size_t  rb_sz;
    uchar_t read_buffer[QEBlockSize];

    size_t  wb_sz;
    uchar_t write_buffer[QEBlockSize];

    size_t  eb_sz;
    uchar_t encrypt_buffer[QEBlockSize];

    FILE* fp;
};


} // namespace Quark
#endif
