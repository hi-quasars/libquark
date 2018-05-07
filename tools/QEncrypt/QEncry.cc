/* *********************
 *  a block-based encryption library
 *
 */

#include <malloc.h>
#include <cerrno>
#include <cstring>
#include <iostream>

#include "QEncry.h"

namespace Quark {

const char** QuarkAppendFile::fpmodes = QAF_Mode_Strs;
PageArray* QuarkAppendFile::page_arena_gptr;

int QuarkAppendFile::initialize() { page_arena_gptr = new PageArray; }
int QuarkAppendFile::deinitialize() {
    for (PageArray::iterator itr = page_arena_gptr->begin();
         itr != page_arena_gptr->end(); ++itr) {
        free(*itr);
    }
    delete page_arena_gptr;
}

ByteBlock* QuarkAppendFile::alloc_append_block(void* memptr) {
    if (!memptr) {
        return NULL;
    }
    ByteBlock* bb = new ByteBlock(memptr, meta.blksize);
    blk_memtable.push_back(bb);
    return bb;
}

ByteBlock* QuarkAppendFile::alloc_append_block() {
    size_t len = meta.blksize;
    void* memptr = malloc(len);
    memset(memptr, 0, len);  //!

    if (!memptr) {
        return NULL;
    }
    ByteBlock* bb = new ByteBlock(memptr, len);
    QuarkAppendFile::page_arena_gptr->push_back(memptr);
    blk_memtable.push_back(bb);
    return bb;
}

void* QuarkAppendFile::AllocBlockBuffer() {
    size_t len = meta.blksize;
    void* memptr = malloc(len);
    memset(memptr, 0, len);  //!
    if (memptr) {
        QuarkAppendFile::page_arena_gptr->push_back(memptr);
    }
    return memptr;
}

int QuarkAppendFile::AppendBlockToBuffer(void* memptr) {
    ByteBlock* b = alloc_append_block(memptr);  //...
    if (b) {
        meta.blkcount++;
        return rOK;
    }
    return rEMEM;
}

int QuarkAppendFile::WriteAllBlocks() {
    int ret;
    for (ByteBlockArray::iterator itr = blk_memtable.begin();
         itr != blk_memtable.end(); ++itr) {
        ret = ByteBlock::Block2File(fp, *itr);
        if (ret != rOK) {
            return ret;
        }
    }

    ret = meta.WriteToFile(fp);

    return ret;
}

int QuarkAppendFile::ReadAllBlocks() {
    int ret = rOK;
    if (!fp) {
        ret = rENVAL;
        goto OUT;
    }
    for (uint64_t i = 0; i < meta.blkcount; i++) {
        ByteBlock* bptr = alloc_append_block();
        if (!bptr) {
            ret = rEMEM;
            goto OUT;
        }
        ret = ByteBlock::File2Block(fp, bptr);
        if (ret != rOK) {
            goto OUT;
        }
    }
OUT:
    return ret;
}

int QuarkAppendFile::BufferToRawString() { return 0; }

QuarkAppendFile* QuarkAppendFile::NewNonEmptyReadFile(const char* f, bool chk) {
    // open
    QuarkAppendFile* qaf = alloc(f, RW);
    // check

    if (chk && !qaf->check(true)) {
        delete qaf;
        return NULL;
    }
    return qaf;
}

QuarkAppendFile* QuarkAppendFile::NewNonEmptyAppendFile(const char* f) {
    QuarkAppendFile* qaf = alloc(f, RW);
    if (qaf && qaf->check(true) && qaf->chmod(APP)) {
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
QuarkAppendFile* QuarkAppendFile::check(bool upd) {
    if (!fp) return NULL;
    struct QFMetaBlock* sp_b = meta.ReadFromFile(fp);
    if (sp_b && 0 == memcmp(meta.magic, sp_b->magic, MGBLKSIZE)) {
        if (upd) {
            meta.blksize = sp_b->blksize;
            meta.blkcount = sp_b->blkcount;
        }
        return this;
    }
    return NULL;
}

// BS BC MG
//
int QuarkAppendFile::QFMetaBlock::WriteToFile(FILE* fp1) {
    int ret;

    BSBLK b1(&blksize, 1);
    BCBLK b2(&blkcount, 1);
    MGBLK b3(magic, MGBLKSIZE);

    ret = BSBLK::Block2File(fp1, &b1);
    if (ret != rOK) goto ERR;

    ret = BCBLK::Block2File(fp1, &b2);
    if (ret != rOK) goto ERR;

    ret = MGBLK::Block2File(fp1, &b3);
    if (ret != rOK) goto ERR;

    return rOK;
ERR:
    return rERR;
}

QuarkAppendFile::QFMetaBlock* QuarkAppendFile::QFMetaBlock::ReadFromFile(
    FILE* fp1) {
    int ret;
    struct QFMetaBlock* qb = new QFMetaBlock(0, 0, "");
    // locate
    long int ft_now = ftell(fp1);
    long int ft_footer;
    BSBLK b1(&(qb->blksize), 1);
    BCBLK b2(&(qb->blkcount), 1);
    MGBLK b3(qb->magic, MGBLKSIZE);

    if (ft_now < 0) {
        // std::cout << "ft1:" << ft_now << std::endl;
        goto ERR;
    }
    fseek(fp1, 0, SEEK_END);
    // std::cout << "ft2:" << ftell(fp1) << std::endl;
    ft_footer = ftell(fp1) - QAF_Footer_Size;
    fseek(fp1, ft_footer, SEEK_SET);
    // std::cout << "ft3:" << ftell(fp1) << std::endl;

    ret = BSBLK::File2Block(fp1, &b1);
    if (ret != rOK) {
        std::cerr << "BSBLK failed:" << ret << std::endl;
        goto ERR;
    }

    ret = BCBLK::File2Block(fp1, &b2);
    if (ret != rOK) {
        std::cerr << "BCBLK failed:" << ret << std::endl;
        goto ERR;
    }

    ret = MGBLK::File2Block(fp1, &b3);
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

QuarkAppendFile* ImportFromPlainFile(const char*, int rw) {}

QuarkAppendFile* ImportFromPlainString(const char* dst, const char* src,
                                       int rw) {}

const char* Cstr[CSTRSIZE] = {"QEncr-92817", "01a9cc7609", "f4d394d5", "e12a3d339"};
const char** QEncryption::key = Cstr;

int QEncryption::EncryptAndPut(std::string& dst, const std::string& src) {
    return enc_dec<CryptoPP::ChaCha20::Encryption>(dst, src, &enc);
}

int QEncryption::EncryptAndPut(char* dst, const char *src, int len) {
    return enc_dec<CryptoPP::ChaCha20::Encryption>(dst, src, len, &enc);
}

int QEncryption::DecryptAndGet(std::string& dst, const std::string& src) {
    return enc_dec<CryptoPP::ChaCha20::Decryption>(dst, src, &dec);
}

int QEncryption::DecryptAndGet(char* dst, const char *src, int len) {
    return enc_dec<CryptoPP::ChaCha20::Decryption>(dst, src, len, &dec);
}

}  // namespace Quark
