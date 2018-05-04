
#ifndef QENCRY_H
#define QENCRY_H

#include <cstdio>
#include <stdint.h>

// TODO - replace vector
#include <vector>

#define DEBUG

namespace Quark {

/*
 *  Encryption:
 *   plaintext           File      ------------------> Mem(Object)
 *   ```
 *    field_0=abcd1         |---->
 *    field_1=abcd2
 *    field_2=abcd2
 *    ...
 *   ```
 *
 *  fp -----> Block ----> EncryptObject::Decrypt ----> Block
 *
 *
 *
 *
 */

typedef uint8_t uchar_t;

const size_t QEBlockSize = 128;
const size_t QEKeySize = 32;
const uchar_t *QEKeys = (uchar_t *)"879b976f9e1d328865559a771b982120";

enum {
  rOK = 0,
  rEOF,
  rERR,
  rENVAL,
  rEMEM
};

class PageArena;
class QuarkAppendFile;
class QAF_Test_Obj;

class BlockBase {};

/*
 * UNIT must be the scalar-type.
 * */
template <typename UNIT = uchar_t, const size_t SZ = QEBlockSize>
class Block : public BlockBase {
  friend class PageArena;
  friend class QuarkAppendFile;
  friend class QAF_Test_Obj;

 public:
  Block() : vl_sz(0), ctnptr(ctn) {
    memset(ctn, 0, SZ); //zero
  }
  ~Block() {}

  Block(UNIT *u, size_t len) : vl_sz(len), ctnptr(u) {}

  static int File2Block(FILE *, Block *, size_t);
  static int File2Block(FILE *, Block *);
  static int Block2File(FILE *, Block *);
  UNIT GetCtn0() { return ctnptr[0]; }
  UNIT GetCtn(int x) { return ctnptr[x]; }
  UNIT *GetCtnX() { return ctnptr; }
  void SetCtn0(UNIT x) {
    ctn[0] = x;
    vl_sz = vl_sz == 0 ? 1 : vl_sz;
  }
  void SetCtn(const UNIT *xptr, size_t len) {
    for (size_t i = 0; i < len && i < SZ; i++) {
      ctn[i] = xptr[i];
    }
    vl_sz = len <= SZ ? len : SZ;
  }
  void SetVlSZ(size_t x) { vl_sz = x; }
  bool operator==(const Block<UNIT, SZ> &b) {
    if (this->vl_sz == b.vl_sz &&
        0 == memcmp(this->ctnptr, b.ctnptr, this->vl_sz))
      return true;
    return false;
  }

 private:
  UNIT ctn[SZ];  // content
  size_t vl_sz;  // valid size
  UNIT *ctnptr;
};

/*
 *  from @fp read @blksize units of @UNIT into @b
 */
template <typename UNIT, const size_t SZ>
int Block<UNIT, SZ>::File2Block(FILE *fp, Block *b, size_t bc) {
  int ret = rENVAL;
  if (fp && b && bc <= SZ) {
    size_t sz = fread((void *)b->ctnptr, sizeof(UNIT), bc, fp);
    if (sz == bc) {
      b->vl_sz = bc;
      ret = rOK;
    } else if (feof(fp)) {
      b->vl_sz = sz;
      ret = rEOF;
    } else {
      ret = rERR;
    }
  }
  return ret;
}

template <typename UNIT, const size_t SZ>
int Block<UNIT, SZ>::File2Block(FILE *fp, Block *b) {
  int ret = rENVAL;
  if (fp && b) {
    // std::cout << "SZ:" << SZ << std::endl;
    size_t sz = fread((void *)b->ctnptr, sizeof(UNIT), SZ, fp);
    // std::cout << "sz:" << sz << std::endl;
    if (sz == SZ) {
      b->vl_sz = SZ;
      ret = rOK;
    } else if (feof(fp)) {
      b->vl_sz = sz;
      ret = rEOF;
    } else {
      ret = rERR;
    }
  }
  return ret;
}

/*
 * from @b write `@b->vl_sz` units of UNIT into @fp
 */
template <typename UNIT, const size_t SZ>
int Block<UNIT, SZ>::Block2File(FILE *fp, Block *b) {
  int ret = rERR;
  if (fp && b) {
    size_t sz = fwrite((void *)b->ctnptr, sizeof(UNIT), b->vl_sz, fp);
    if (sz == b->vl_sz) {
      ret = rOK;
    }
  }
  return ret;
}

// mem <--> Block
// Block don't maintain the area.
// zero-copy block
template <>
class Block<void, 1> : public BlockBase {
  friend class PageArena;
  friend class QuarkAppendFile;

 public:
  Block(void *ctnptr_, size_t sz_) : ctnptr(ctnptr_), cap(sz_) {}
  ~Block() {}
  void SetVlSZ(size_t sz_) { vl_sz = sz_; }
  static int File2Block(FILE *, Block *, size_t);
  static int File2Block(FILE *, Block *);
  static int Block2File(FILE *, Block *);
  size_t GetVlSZ() { return vl_sz;}
  size_t GetCap() { return cap; }
  void * GetCtn() { return ctnptr; }
 private:
  size_t vl_sz;
  size_t cap;
  void *ctnptr;
};

int Block<void, 1>::File2Block(FILE *fp, Block *b, size_t sz) {
  int ret = rENVAL;
  if (fp && b && sz < b->cap) {
    size_t rsz = fread(b->ctnptr, 1, sz, fp);
    if (rsz == sz) {
      b->vl_sz = sz;
      ret = rOK;
    } else if (feof(fp)) {
      b->vl_sz = sz;
      ret = rEOF;
    } else {
      ret = rERR;
    }
  }
  return ret;
}

int Block<void, 1>::File2Block(FILE *fp, Block *b) {
  int ret = rENVAL;
  if (fp && b) {
    size_t rsz = fread(b->ctnptr, 1, b->cap, fp);
    if (rsz == b->cap) {
      b->vl_sz = b->cap;
      ret = rOK;
    } else if (feof(fp)) {
      b->vl_sz = rsz;
      ret = rEOF;
    } else {
      ret = rERR;
    }
  }
  return ret;
}

int Block<void, 1>::Block2File(FILE *fp, Block *b) {
  int ret = rERR;
  if (fp && b) {
    size_t sz = fwrite(b->ctnptr, 1, b->cap, fp);
    if (sz == b->cap) {
      ret = rOK;
    }
  }
  return ret;
}

class PageArena {
 public:
   void *Alloc(size_t bs) { }
};

typedef Block<void, 1> ByteBlock;
typedef std::vector<ByteBlock *> ByteBlockArray;
typedef std::vector<void *> PageArray;

const size_t QAF_BS = QEBlockSize;
const char *QAF_Magic =
    "QuarkFS-95fb52146c042eeccb65094f92b306a0";  // output of `echo "quark-fs" |
                                                 // md5sum`
const int QAF_Footer_Size = 64;                  // bytes
const char *QAF_Mode_Strs[] = {"w", "r+", "a"};

PageArena *QAF_page_arena;

/*
 * a QuarkFile ---> PlainFile
 *  | Block0 | Block1 | ... | Blockn | Footer |
 *
 */
class QuarkAppendFile {
 public:
  friend class QAF_Test_Obj;

  static QuarkAppendFile *NewNonEmptyReadFile(const char *f, bool chk = true);
  static QuarkAppendFile *NewNonEmptyAppendFile(const char *f);
  static QuarkAppendFile *NewEmptyAppendFile(const char *f);
  static QuarkAppendFile *ImportFromPlainFile(const char *, int rw);
  static QuarkAppendFile *ImportFromPlainString(const char *dst,
                                                const char *src, int rw);
  static int initialize();
  static int deinitialize();

 private:
  ByteBlock* alloc_append_block();
  ByteBlock* alloc_append_block(void *memptr);

 public:
  QuarkAppendFile(const char *fname_)
      : fname(fname_), fp(NULL), meta(QAF_BS, 0, QAF_Magic) {}
  ~QuarkAppendFile() {
    if (fp) {
      fclose(fp);
    }
    for (ByteBlockArray::iterator itr = blk_memtable.begin();
        itr != blk_memtable.end(); ++itr) {
        delete *itr;
    }
  }

  /*
   *       
   *     
   *  raw ------> buffer ------> file
   *
   *
   *  raw <------ buffer <------ file
   *
   * */
  void *AllocBlockBuffer();
  size_t GetBlockBufferSize() { return meta.blksize; }

  int AppendBlockToBuffer(void* memptr);
  int WriteAllBlocks();
  
  int BufferToRawString();
  int ReadAllBlocks();
  
  FILE *GetFP() { return fp; }

 private:
  static QuarkAppendFile *alloc(const char *, int);
  QuarkAppendFile *chmod(int mode);
  QuarkAppendFile *check(bool);

  enum ModeBit {
    CREAT = 0,  //
    RW,         //
    APP         //
  };
  static const char **fpmodes;

#define BSBLKSIZE sizeof(uint32_t)
#define BCBLKSIZE sizeof(uint64_t)
#define MGBLKSIZE  \
  (QAF_Footer_Size - BSBLKSIZE - BCBLKSIZE)
  struct QFMetaBlock {  // QuarkFile Metadata Block
    typedef Block<uint32_t, 1> BSBLK;
    typedef Block<uint64_t, 1> BCBLK;
    typedef Block<char, MGBLKSIZE>
        MGBLK;

    uint32_t blksize;
    uint64_t blkcount;
    char magic[MGBLKSIZE];

    /*
    BSBLK blksize;
    BCBLK blkcount;
    MGBLK magic;
    */

    QFMetaBlock(size_t bs, size_t bc, const char *m) :
      blksize(bs), blkcount(bc)
      {
        strncpy(magic, m, MGBLKSIZE);
    }
    ~QFMetaBlock() {}

    /*
     * | Block0 | Block1 | ... | Blockn | Footer |
     */
    int WriteToFile(FILE *fp1);
    struct QFMetaBlock *ReadFromFile(FILE *fp1);

  } meta;

  FILE *fp;
  std::string fname;
  int mode;

  static PageArray* page_arena_gptr;
  ByteBlockArray blk_memtable;
};

/*
 *
 */
class QEncryption {
 public:
  int EncryptAndPut(const std::string &src) {}
  int DecryptAndGet(std::string &dst) {}
};

}  // namespace Quark
#endif
