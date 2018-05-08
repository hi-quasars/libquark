#ifndef LibQencrypt_H
#define LibQencrypt_H
#ifdef __cplusplus
extern "C" {
#endif

struct qaf_handle_s;
typedef struct qaf_handle_s qaf_handle_t;
enum { rOK = 0, rEOF, rERR, rENVAL, rEMEM };

int qaf_initialize();
int qaf_deinitialize();

qaf_handle_t* OpenNonEmptyReadFile(const char *filename);
qaf_handle_t* OpenNonEmptyAppendFile(const char *filename);
qaf_handle_t* OpenEmptyAppendFile(const char *filename);
void CloseFile(qaf_handle_t* fh);

void* AllocBuffer(qaf_handle_t* fh);
int   BufferSize(qaf_handle_t* fh);
int   SecureAppend(qaf_handle_t* fh, void *ptr);
int   Sync(qaf_handle_t* fh);

int   SecureRead(qaf_handle_t* fh);
int   BlockCount(qaf_handle_t* fh);
int   BlockSize(qaf_handle_t* fh);
const char* Block(qaf_handle_t* fh, int idx);

#ifdef __cplusplus
} //extern "C"
#endif
#endif
