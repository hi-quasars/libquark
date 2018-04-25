
/* *********************
 *  a block-based encryption library
 *
 * */

#include <iostream>
#include <cstring>
#include <cerrno>

#include "QEncry.h"

namespace Quark {

QEncryObject::QEncryObject(const char *filename)
    : fp(NULL), rb_sz(0), wb_sz(0), encr_key(QEKeys), encr_key_len(QEKeySize) {
  if (NULL == (fp = fopen(filename, "rb+"))) {
    std::cerr << "fopen:" << filename << "failed:" << strerror(errno)
              << std::endl;
  }
  if (encr_key_len != strlen((char *)encr_key)) {
    std::cerr << "warn: key length not match!" << std::endl;
    encr_key_len = strlen((char *)encr_key);
  }
  chacha20poly1305_init(&encr_adea_ctx, encr_key, encr_key_len);
}

QEncryObject::~QEncryObject() {
  if (fp != NULL) {
    fclose(fp);
  }
}

int QEncryObject::ReadBlock(void *buf) {
  int ret = internal_read_rbuffer();
  if (ret != rERR && buf) {
    memcpy((uchar_t *)buf, read_buffer, rb_sz * sizeof(uchar_t));
  }
  return ret;
}

int QEncryObject::internal_read_rbuffer() {
  state = rERR;

  if (NULL != fp) {
    size_t sz = fread((void *)read_buffer, sizeof(uchar_t), QEBlockSize, fp);
    if (sz == QEBlockSize) {
      rb_sz = sz;
      state = rOK;
    } else if (feof(fp)) {
      rb_sz = sz;
      state = rEOF;
    } else {
      // error occur
    }
  }
  return state;
}

int QEncryObject::WriteBlock(void *buf, size_t len) {
  if (len <= QEBlockSize && buf) {
    memcpy((uchar_t *)write_buffer, buf, len * sizeof(uchar_t));
    wb_sz = len;
  }

  return internal_write_wbuffer();
}

int QEncryObject::internal_write_wbuffer() {
  state = rERR;
  if (NULL != fp) {
    size_t sz = fwrite((void *)write_buffer, sizeof(uchar_t), wb_sz, fp);
    if (sz == wb_sz) {
      state = rOK;
    } else {
      // error occur
    }
  }
  return state;
}

void QEncryObject::internal_encript(uchar_t *from, size_t len,
                                    int en_or_de) {  // read_buffer --> encrypt
                                                     // --> write_buffer
  int seqnr = 100;
  int addlen = 4;
  int is_encry = en_or_de;
  uint32_t enc_len;
  int ret;

  if (len < QEBlockSize) {

    // chacha20
    chacha20poly1305_crypt(&encr_adea_ctx, seqnr, encrypt_buffer, from, len,
                           addlen, is_encry);
#ifdef DEBUG
    ret = chacha20poly1305_get_length(&encr_adea_ctx, &enc_len, seqnr,
                                      write_buffer, 4);
    std::cout << "get_length: " << ret << std::endl;
    if (enc_len != encr_key_len) {
      std::cerr << "warning: mismatch length! [after,before] " << enc_len << ";"
                << encr_key_len << std::endl;
    }
#endif
    wb_sz = (size_t)enc_len;
  }
}

void QEncryObject::QkEncript(uchar_t *from, size_t len) {
  return internal_encript(from, len, 1);
}

void QEncryObject::QkDecript(uchar_t *from, size_t len) {
  return internal_encript(from, len, 0);
}

#ifdef DEBUG
void QEncryObject::PrintUchar(uchar_t *ptr, size_t len) {
  for (size_t i = 0; i < len; i++) {
    std::cout << (int)(ptr[i]) << " ";
  }
  std::cout << std::endl;
}
#endif

}  // namespace Quark
