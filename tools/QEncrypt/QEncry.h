
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

const size_t QEBlockSize = 512;
const size_t QEKeySize = 32;
const uchar_t *QEKeys = (uchar_t *)"879b976f9e1d328865559a771b982120";
const char *QFMagic = "95fb52146c042eeccb65094f92b306a0"; // output of `echo "quark-fs" | md5sum`


enum {
  rOK = 0,
  rEOF,
  rERR,
  rENVAL
};

class BlockArena;
class QuarkAppendFile;

class BlockBase {
};


template <typename UNIT = uchar_t, const size_t SZ = QEBlockSize>
class Block : public BlockBase {
  friend class BlockArena;
  friend class QuarkAppendFile;
 public:
  Block() : vl_sz(0), ctnptr(ctn) {  }
  ~Block() {}
  
  Block(UNIT* u, size_t len) : vl_sz(len), ctnptr(u){
  }

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
class Block <void, 1> : public BlockBase {
  friend class BlockArena; 
  friend class QuarkAppendFile;
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

typedef Block<void, 1> ByteBlock;
typedef vector<ByteBlock *> ByteBlockArray;


/*
 * a QuarkFile ---> PlainFile
 *  | Block0 | Block1 | ... | Blockn | Footer |
 *
 */
class QuarkAppendFile {
public:
    /*
     * Read-Append file (from an existing quark-file)
     * failed if file not-exited or not valid format.
     */
    static QuarkAppendFile* NewReadFile(const char*);   
    
    /*
     * Write-Append file (to a non-exist file in quark-file format);
     * failed if file already exist.
     */
    static QuarkAppendFile* NewWriteFile(const char*);
    static QuarkAppendFile* ImportFromPlainFile(const char*, int rw);
    


    QuarkAppendFile(const char* fname_) : fname(fname_), fp(NULL) { }
    ~QuarkAppendFile() { 
      if(fp){
        fclose(fp);
      }
    }

    int Read1Block();        // fp ---> a block ---> blk_memtable
    int Write1Block();       // blk_memtable --> a block --> fp
private:
    enum ModeBit{
      BITRW = 0
    };
    struct QFMetaBlock {                 // QuarkFile Metadata Block
        uint32_t blksize;
        uint64_t blkcount;
        char magic[32];

        QFMeta(size_t bs, size_t bc) 
            : blksize(bs), blkcount(bc){ 
            
            }
        ~QFMeta() {}
    } meta;

    
    FILE*           fp;
    std::string     fname;
    int             mode;
    
    ByteBlockArray  blk_memtable;
};

/*
 * 
 */
class QEncryption{
public:
  int EncryptAndPut(const std::string &src) {}
  int DecryptAndGet(std::string &dst) {}
};



}  // namespace Quark
#endif
