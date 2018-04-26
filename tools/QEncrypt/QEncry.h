
#ifndef QENCRY_H
#define QENCRY_H

#include <cstdio>
#include <stdint.h>

// 3rdparty
#include "chacha20poly1305/chacha.h"
#include "chacha20poly1305/chachapoly_aead.h"
#include "chacha20poly1305/poly1305.h"

#define DEBUG

namespace Quark {

typedef uint8_t uchar_t;

const size_t QEBlockSize = 512;
const size_t QEKeySize = 32;
const uchar_t *QEKeys = (uchar_t *)"879b976f9e1d328865559a771b982120";
enum {
  rOK = 0,
  rEOF,
  rERR,
  rENVAL
};

class BlockArena;

template <typename UNIT = uchar_t, const size_t SZ = QEBlockSize>
class Block {
  friend class BlockArena;
 public:
  Block() : vl_sz(0), ctnptr(ctn) {  }
  ~Block() {}
  
  static int File2Block(FILE *, Block *, size_t); 
  static int Block2File(FILE *, Block *);
 private:
  UNIT ctn[SZ];  // content
  size_t vl_sz;  // valid size
  UNIT *ctnptr;
};

/*
 *  from @fp read @blksize units of @UNIT into @b
 */
template <typename UNIT, const size_t SZ>
int Block<UNIT, SZ>::File2Block(FILE *fp, Block *b, size_t blksize) {
  int ret = rENVAL;
  if (fp && b && blksize < SZ) {
    size_t sz = fread((void *)b->ctnptr, sizeof(UNIT), blksize, fp);
    if (sz == blksize) {
      b->vl_sz = blksize;
      ret = rOK;
    } else if (feof(fp)){
      b->vl_sz = blksize;
      ret = rEOF;
    }
    ret = rERR;
  }
  return ret;
}

/*
 * from @b write `@b->vl_sz` units of UNIT into @fp
 */
template <typename UNIT, const size_t SZ>
int Block<UNIT, SZ>::Block2File(FILE *fp, Block *b) {
  int ret = rERR;
  if(fp && b) {
    size_t sz = fwrite((void *)b->ctnptr, sizeof(UNIT), b->vl_sz, fp);
    if (sz == b->vl_sz) {
      ret = rOK;
    }
  }
  return ret;
}

// mem <--> Block
template <>
class Block <void, 1> {
  friend class BlockArena; 
public:
  Block(void* ctnptr_, size_t sz_) 
      : ctnptr(ctnptr_), cap(sz_)
    {
    }
  ~Block() {
  }
  void SetValidSize(size_t sz_) { vl_sz = sz_; }
  static int File2Block(FILE *, Block *, size_t);
  static int Block2File(FILE *, Block *);

private:
  size_t vl_sz;
  size_t cap;
  void *ctnptr;
};

int Block <void, 1>::File2Block(FILE *fp, Block *b, size_t sz) {
  int ret = rENVAL;           
  if (fp && b && sz < b->cap) {
    size_t rsz = fread(b->ctnptr, 1, sz, fp);
    if (rsz == sz) {
       b->vl_sz = sz;
       ret = rOK;
    } else if (feof(fp)) {
       b->vl_sz = sz;
       ret = rEOF;
    }
    ret = rERR;
  }
  return ret;
}

int Block <void, 1>::Block2File(FILE *fp, Block *b) {
  int ret = rERR;
  if(fp && b) {  
    size_t sz = fwrite(b->ctnptr, 1, b->vl_sz, fp);
    if(sz == b->vl_sz) {
      ret = rOK;
    }
  }
  return ret;
}



class BlockArena {
public:


};

class QEncryObject {
 public:
  QEncryObject(const char *filename);
  ~QEncryObject();

  int ReadBlock(void *buf);               // Read a block and decrypt
  int WriteBlock(void *buf, size_t len);  // encript and write a line

  void QkEncript(uchar_t *from, size_t len);
  void QkDecript(uchar_t *from, size_t len);
#ifdef DEBUG
  void PrintUchar(uchar_t *ptr, size_t len);
#endif
  uchar_t *InternalBuffer(int type) {
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
  // int internal_read_file(uchar_t* buf, size_t sz);

  // encription materials
  struct chachapolyaead_ctx encr_adea_ctx;
  const uchar_t *encr_key;
  size_t encr_key_len;

  // other meta
  int state;
  size_t rb_sz;
  uchar_t read_buffer[QEBlockSize];

  size_t wb_sz;
  uchar_t write_buffer[QEBlockSize];

  size_t eb_sz;
  uchar_t encrypt_buffer[QEBlockSize];

  FILE *fp;
};

}  // namespace Quark
#endif
