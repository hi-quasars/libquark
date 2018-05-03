
/* *********************
 *  a block-based encryption library
 *
 */

#include <iostream>
#include <cstring>
#include <cerrno>

#include "QEncry.h"

namespace Quark {

const char **QuarkAppendFile::fpmodes = QAF_Mode_Strs;
QuarkAppendFile* QuarkAppendFile::NewNonEmptyReadFile(const char* f, bool chk) {
  // open
  QuarkAppendFile* qaf = alloc(f, RW);
  // check

  if (chk && !qaf->check()) {
    delete qaf;
    return NULL;
  }
  return qaf;
}

QuarkAppendFile* QuarkAppendFile::NewNonEmptyAppendFile(const char* f) {
  QuarkAppendFile* qaf = alloc(f, RW);
  if (qaf && qaf->check() && qaf->chmod(APP)) {
    return qaf;
  }
  delete qaf;
  return NULL;
}

QuarkAppendFile* QuarkAppendFile::NewEmptyAppendFile(const char* f) {
  // open
  QuarkAppendFile* qaf = alloc(f, CREAT);
  if (qaf->chmod(APP)) {
    return qaf;
  }
  delete qaf;
  return NULL;
}

QuarkAppendFile* QuarkAppendFile::alloc(const char* f, int mode) {
  QuarkAppendFile* ret = new QuarkAppendFile(f);
  ret->mode = mode;

  ret->fp = fopen(f, fpmodes[mode]);
  if (NULL == ret->fp) {
    delete ret;
    return NULL;
  }
  return ret;
}

//
//
QuarkAppendFile* QuarkAppendFile::chmod(int mode) {
  this->fp = freopen(this->fname.c_str(), fpmodes[mode], this->fp);
  return this->fp ? this : NULL;
}

// success - return @this
// failed  - return NULL
QuarkAppendFile* QuarkAppendFile::check() {
  if (!fp) 
      return NULL;
  struct QFMetaBlock* sp_b = meta.ReadFromFile(fp);
  if (!sp_b || meta.magic == sp_b->magic) {
    return NULL;
  }
  return this;
}

// BS BC MG
// 
int QuarkAppendFile::QFMetaBlock::WriteToFile(FILE* fp1) {
  int ret;

  ret = BSBLK::Block2File(fp1, &blksize);
  if (ret != rOK) goto ERR;

  ret = BCBLK::Block2File(fp1, &blkcount);
  if (ret != rOK) goto ERR;

  ret = MGBLK::Block2File(fp1, &magic);
  if (ret != rOK) goto ERR;

  return rOK;
ERR:
  return rERR;
}

QuarkAppendFile::QFMetaBlock* QuarkAppendFile::QFMetaBlock::ReadFromFile(FILE* fp1) {
  int ret;
  struct QFMetaBlock* qb = new QFMetaBlock(0, 0, "");
  // locate
  long int ft_now = ftell(fp1);
  long int ft_footer;
  if(ft_now < 0) {
    //std::cout << "ft1:" << ft_now << std::endl;
    goto ERR;
  }
  fseek(fp1, 0, SEEK_END);
  //std::cout << "ft2:" << ftell(fp1) << std::endl;
  ft_footer = ftell(fp1) - QAF_Footer_Size;
  fseek(fp1, ft_footer, SEEK_SET);
  //std::cout << "ft3:" << ftell(fp1) << std::endl;
  

  ret = BSBLK::File2Block(fp1, &(qb->blksize));
  if (ret != rOK){
      std::cerr << "BSBLK failed:" << ret << std::endl;
      goto ERR;
  }

  ret = BCBLK::File2Block(fp1, &(qb->blkcount));
  if (ret != rOK) { 
      std::cerr << "BCBLK failed:" << ret << std::endl;
      goto ERR;
  }

  ret = MGBLK::File2Block(fp1, &(qb->magic));
  if (ret != rOK && ret != rEOF) { 
      std::cerr << "MGBLK failed:" << ret << std::endl;
      goto ERR;
  }

  fseek(fp1, ft_now, SEEK_SET);
  return qb;
ERR:
  delete qb;
  return NULL;
}

int QuarkAppendFile::ReadBlock() {}

int QuarkAppendFile::AppendBlock() {}

QuarkAppendFile* ImportFromPlainFile(const char*, int rw) {}

QuarkAppendFile* ImportFromPlainString(const char* dst, const char* src,
                                       int rw) {}



}  // namespace Quark
