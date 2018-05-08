#include <QEncry.h>
#include <libqencrypt.h>

using Quark::QuarkAppendFile;

extern "C" {

struct qaf_handle_s {
    QuarkAppendFile *f;
};

int qaf_initialize() {
    QuarkAppendFile::initialize();
}

int qaf_deinitialize(){
    QuarkAppendFile::deinitialize();
}

qaf_handle_t* OpenNonEmptyReadFile(const char *filename) {
    qaf_handle_t* fh = new qaf_handle_t;
    fh->f = QuarkAppendFile::NewNonEmptyReadFile(filename, true);
    if ((void*)(NULL) == fh->f) {
        delete fh;
        return NULL;
    }
    return fh;
}

qaf_handle_t* OpenNonEmptyAppendFile(const char *filename) {
    qaf_handle_t* fh = new qaf_handle_t;
    fh->f = QuarkAppendFile::NewNonEmptyAppendFile(filename);
    if ((void*)(NULL) == fh->f) {
        delete fh;
        return NULL;
    }
    return fh;
}

qaf_handle_t* OpenEmptyAppendFile(const char *filename) {
    qaf_handle_t* fh = new qaf_handle_t;
    fh->f = QuarkAppendFile::NewEmptyAppendFile(filename);
    if ((void*)(NULL) == fh->f) {
        delete fh;
        return NULL;
    }
    return fh;
}

void CloseFile(qaf_handle_t* fh) {
    delete fh->f;
    delete fh;
}

void* AllocBuffer(qaf_handle_t* fh) {
    if(fh) {
        return fh->f->AllocBlockBuffer();
    }
    return NULL;
}
int   BufferSize(qaf_handle_t* fh) {
    if(fh) {
        return fh->f->GetBlockBufferSize();
    }
    return 0;
}
int   SecureAppend(qaf_handle_t* fh, void *ptr){
    if(fh && ptr) {
        return fh->f->AppendBlockToBufferEnc(ptr);
    }
    return -rENVAL;
}
int   Sync(qaf_handle_t* fh) {
    if(fh) {
        return fh->f->WriteAllBlocks();
    }
    return -rENVAL;
}

int   SecureRead(qaf_handle_t* fh) {
    if(fh) {
        return fh->f->ReadAllBlocksDec();
    }
    return -rENVAL;
}

int   BlockCount(qaf_handle_t* fh) {
    if(fh) {
        return fh->f->GetBlockCount();
    }
    return -rENVAL;
}

int   BlockSize(qaf_handle_t* fh) {
    if(fh) {
        return fh->f->GetBlockBufferSize();
    }
    return -rENVAL;
}
const char* Block(qaf_handle_t* fh, int idx) {
    if(fh) {
        return fh->f->GetBlock(idx);
    }
    return NULL;
}

}  // extern "C"
