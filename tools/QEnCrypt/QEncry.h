#include <string>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cerrno>

#ifndef QENCRY_H
#define QENCRY_H

const int CQEBufSize = 512;

class QEncryObject {
public:
    QEncryOBject(const char *filename) : buffer(NULL), fp(NULL) { 
        if ( NULL == (fp = fopen(filename, "r")) {
            std::cerr << "fopen:" <<  filename << "failed:" << strerror(errno) << std::endl;
        }
    }

    ~QEncryOBject(){ 
        if ( fp != NULL ){
            fclose(fp);
        }
    }

    int ReadLineDec(void *buf);     // Read a line and decrypt
    int WriteLineEnc(void *buf);             // encript and write a line

private:
    char buffer[CQEBufSize];
    FILE* fp;
};

#endif
