#include "gtest/gtest.h"
#include "QEncry.h"

#include <cstddef>
#include <string>

std::string dirname = "tests/";
std::string suffix = "tst.txt";

/* tests for block-io */

TEST(BlockIO, one_byte_io) {

  std::string str1 = dirname + "BlockIO" + suffix;
  char x = 'a';
  Quark::Block<char, 1> B0(&x, 1);
  Quark::QuarkAppendFile *f1 =
      Quark::QuarkAppendFile::NewEmptyAppendFile(str1.c_str());

  EXPECT_NE(f1, (void *)0);

  int re = Quark::Block<char, 1>::Block2File(f1->GetFP(), &B0);
  EXPECT_EQ(re, 0);

  delete f1;

  Quark::Block<char, 1> B1;
  Quark::QuarkAppendFile *f =
      Quark::QuarkAppendFile::NewNonEmptyReadFile(str1.c_str(), false);

  re = Quark::Block<char, 1>::File2Block(f->GetFP(), &B1, sizeof(char));
  char r = B1.GetCtn0();

  std::cout << "return:" << re << " r's val:" << r << std::endl;

  EXPECT_EQ(r, x);

  delete f;
}

TEST(BlockIO, multibyte_io) {
  uint32_t k = 80;
  std::string str1 = dirname + "BlockIO02" + suffix;
  Quark::Block<uint32_t, 1> B0(&k, 1);
  Quark::QuarkAppendFile *f =
      Quark::QuarkAppendFile::NewEmptyAppendFile(str1.c_str());

  EXPECT_NE(f, (void *)0);

  int re = Quark::Block<uint32_t, 1>::Block2File(f->GetFP(), &B0);
  EXPECT_EQ(re, 0);

  delete f;
  Quark::QuarkAppendFile *nf =
      Quark::QuarkAppendFile::NewNonEmptyReadFile(str1.c_str(), false);
  Quark::Block<uint32_t, 1> B1;
  re = Quark::Block<uint32_t, 1>::File2Block(nf->GetFP(), &B1, 1);
  // EXPECT_EQ(re, 0);

  uint32_t j = B1.GetCtn0();
  std::cout << j << std::endl;
  EXPECT_EQ(j, k);
}

TEST(BlockIO, multibyte_arr_io) {
  uint32_t ak[] = {80, 99, 101};
  std::string str1 = dirname + "multibytes_array_io" + suffix;
  Quark::Block<uint32_t, 3> B0(ak, 3);
  Quark::QuarkAppendFile *f =
      Quark::QuarkAppendFile::NewEmptyAppendFile(str1.c_str());

  EXPECT_NE(f, (void *)0);

  int re = Quark::Block<uint32_t, 3>::Block2File(f->GetFP(), &B0);
  EXPECT_EQ(re, 0);

  delete f;
  Quark::QuarkAppendFile *nf =
      Quark::QuarkAppendFile::NewNonEmptyReadFile(str1.c_str(), false);
  Quark::Block<uint32_t, 3> B1;
  re = Quark::Block<uint32_t, 3>::File2Block(nf->GetFP(), &B1, 3);
  // EXPECT_EQ(re, 0);

  for (int i = 0; i < 3; i++) {
    uint32_t j = B1.GetCtn(i);
    std::cout << j << ",";
    EXPECT_EQ(j, ak[i]);
  }
  std::cout << std::endl;
}
TEST(BlockIO, void_block_01) {
  std::string str1 = dirname + "void_block_io" + suffix;
  char buf[50] = "hellobyteblocks//;;kf";
  memset(buf, 0, 50);
  std::cout << " buf : " << buf << std::endl;
  Quark::ByteBlock b1(buf, 50);

  Quark::QuarkAppendFile *f =
      Quark::QuarkAppendFile::NewEmptyAppendFile(str1.c_str());
  int re = Quark::ByteBlock::Block2File(f->GetFP(), &b1);

  std::cout << "re:" << re << std::endl;

  delete f;

  char buf2[50];
  memset(buf2, 0, 50);
  Quark::ByteBlock b2(buf2, 50);
  Quark::QuarkAppendFile *f1 =
      Quark::QuarkAppendFile::NewNonEmptyReadFile(str1.c_str(), false);
  re = Quark::ByteBlock::File2Block(f->GetFP(), &b2);
  std::cout << "re:" << re << std::endl;
  std::cout << " b2.vl_sz :" << b2.GetVlSZ() << std::endl;
  std::cout << " b2.cap :" << b2.GetCap() << std::endl;
  std::cout << " buf2: " << buf2 << std::endl;
  EXPECT_EQ(0, strcmp(buf, buf2));
}

namespace Quark {
/* tests for QFMetaBlock */
class QAF_Test_Obj {
 public:
  static void tst_QFMetaBlock_mem_init() {
    std::string str1 = dirname + "QFmetablock_0" + suffix;
    QuarkAppendFile *f1x = QuarkAppendFile::NewEmptyAppendFile(str1.c_str());
    std::cout << "meta magic: " << f1x->meta.magic << std::endl;
    std::cout << "bs:" << f1x->meta.blksize << std::endl;
    std::cout << "bc:" << f1x->meta.blkcount << std::endl;
    delete f1x;
  }
  static void tst_QFMetaBlock_IO() {
    std::string str1 = dirname + "QFmetablock_1" + suffix;
    QuarkAppendFile *f1x = QuarkAppendFile::NewEmptyAppendFile(str1.c_str());
    std::cout << "meta magic: " << f1x->meta.magic << std::endl;
    std::cout << "bs:" << f1x->meta.blksize << std::endl;
    std::cout << "bc:" << f1x->meta.blkcount << std::endl;
    // std::cout << "info: " << f1x->meta.magic.vl_sz << "," << QAF_Footer_Size
    // - sizeof(uint32_t) - sizeof(uint64_t) << std::endl;
    int ret = f1x->meta.WriteToFile(f1x->fp);
    // std::cout << ret << std::endl;
    delete f1x;

    QuarkAppendFile *f2x =
        QuarkAppendFile::NewNonEmptyReadFile(str1.c_str(), false);
    QuarkAppendFile::QFMetaBlock *qm = f2x->meta.ReadFromFile(f2x->fp);
    EXPECT_NE(qm, (void *)NULL);
    std::cout << "-------------read from file---------------" << std::endl;
    std::cout << "meta magic: " << qm->magic << std::endl;
    std::cout << "bs:" << qm->blksize << std::endl;
    std::cout << "bc:" << qm->blkcount << std::endl;
    delete f2x;
    delete qm;
  }
  static void tst_QAF_01() {
    std::string str1 = dirname + "QAF_01" + suffix;

    QuarkAppendFile *f1x = QuarkAppendFile::NewEmptyAppendFile(str1.c_str());
    std::cout << "meta magic: " << f1x->meta.magic << std::endl;
    std::cout << "bs:" << f1x->meta.blksize << std::endl;
    std::cout << "bc:" << f1x->meta.blkcount << std::endl;
    // std::cout << "info: " << f1x->meta.magic.vl_sz << "," << QAF_Footer_Size
    // - sizeof(uint32_t) - sizeof(uint64_t) << std::endl;
    int ret = f1x->meta.WriteToFile(f1x->fp);
    // std::cout << ret << std::endl;
    delete f1x;

    QuarkAppendFile *f2x =
        QuarkAppendFile::NewNonEmptyReadFile(str1.c_str(), false);

    QuarkAppendFile *chk = f2x->check(false);
    EXPECT_EQ(chk, f2x);

    delete f2x;
  }
  static void tst_File_01() {
    int ret;
    char *bufx;
    std::string str1 = dirname + "file_01" + suffix;
    Quark::QuarkAppendFile::initialize();
    Quark::QuarkAppendFile *fx =
        Quark::QuarkAppendFile::NewEmptyAppendFile(str1.c_str());
    char *opbuf1 = (char *)fx->AllocBlockBuffer();
    size_t sz = fx->GetBlockBufferSize();
    char x = 'A';
    for (size_t i = 0; i < sz && i < 10; i++) {
      opbuf1[i] = x;
      x++;
    }
    ret = fx->AppendBlockToBuffer(opbuf1);
    std::cout << "----------- Write ------------" << std::endl;
    std::cout << "Write(buffer):" << ret << std::endl;
    ret = fx->WriteAllBlocks();
    std::cout << "WriteFile:" << ret << std::endl;

    std::cout << "meta magic: " << fx->meta.magic << std::endl;
    std::cout << "bs:" << fx->meta.blksize << std::endl;
    std::cout << "bc:" << fx->meta.blkcount << std::endl;

    delete fx;
    
    std::cout << "----------- Read ------------" << std::endl;
    Quark::QuarkAppendFile *fx_read = Quark::QuarkAppendFile::NewNonEmptyReadFile(str1.c_str(), true);
    
    EXPECT_NE(fx_read, (void*)NULL);

    std::cout << "meta magic: " << fx_read->meta.magic << std::endl;
    std::cout << "bs:" << fx_read->meta.blksize << std::endl;
    std::cout << "bc:" << fx_read->meta.blkcount << std::endl;
    
    ret = fx_read->ReadAllBlocks();
    std::cout << "read result:" << ret << std::endl;
    size_t ix = 0;
    for(ByteBlockArray::iterator itr = fx_read->blk_memtable.begin();
        itr != fx_read->blk_memtable.end();
        ++itr, ++ix) {
        bufx = (char* )(*itr)->GetCtn();
        std::cout << ix << ":" << bufx << std::endl;
    }

    delete fx_read;
    
    Quark::QuarkAppendFile::deinitialize();
  

  }
};
}

TEST(QAF, metablock1) { Quark::QAF_Test_Obj::tst_QFMetaBlock_mem_init(); }
TEST(QAF, metablock2) { Quark::QAF_Test_Obj::tst_QFMetaBlock_IO(); }
TEST(QAF, metablock3) { Quark::QAF_Test_Obj::tst_QAF_01(); }
TEST(QAFExternal, fileoperation) { Quark::QAF_Test_Obj::tst_File_01(); }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
