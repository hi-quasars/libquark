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

namespace Quark {
/* tests for QFMetaBlock */
class QAF_Test_Obj {
 public:
  static void tst_QFMetaBlock_mem_init() {
    std::string str1 = dirname + "QFmetablock_0" + suffix;
    QuarkAppendFile *f1x = QuarkAppendFile::NewEmptyAppendFile(str1.c_str());
    std::cout << "meta magic: " << f1x->meta.magic.GetCtnX() << std::endl;
    std::cout << "bs:" << f1x->meta.blksize.GetCtn(0) << std::endl;
    std::cout << "bc:" << f1x->meta.blkcount.GetCtn(0) << std::endl;
    delete f1x;
  }
  static void tst_() {}
};
}

TEST(QAF, metablock) { Quark::QAF_Test_Obj::tst_QFMetaBlock_mem_init(); }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
