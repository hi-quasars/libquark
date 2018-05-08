#include "QEncry.h"
#include "gtest/gtest.h"

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
        QuarkAppendFile *f1x =
            QuarkAppendFile::NewEmptyAppendFile(str1.c_str());
        std::cout << "meta magic: " << f1x->meta.magic << std::endl;
        std::cout << "bs:" << f1x->meta.blksize << std::endl;
        std::cout << "bc:" << f1x->meta.blkcount << std::endl;
        delete f1x;
    }
    static void tst_QFMetaBlock_IO() {
        std::string str1 = dirname + "QFmetablock_1" + suffix;
        QuarkAppendFile *f1x =
            QuarkAppendFile::NewEmptyAppendFile(str1.c_str());
        std::cout << "meta magic: " << f1x->meta.magic << std::endl;
        std::cout << "bs:" << f1x->meta.blksize << std::endl;
        std::cout << "bc:" << f1x->meta.blkcount << std::endl;
        // std::cout << "info: " << f1x->meta.magic.vl_sz << "," <<
        // QAF_Footer_Size
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

        QuarkAppendFile *f1x =
            QuarkAppendFile::NewEmptyAppendFile(str1.c_str());
        std::cout << "meta magic: " << f1x->meta.magic << std::endl;
        std::cout << "bs:" << f1x->meta.blksize << std::endl;
        std::cout << "bc:" << f1x->meta.blkcount << std::endl;
        // std::cout << "info: " << f1x->meta.magic.vl_sz << "," <<
        // QAF_Footer_Size
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
        Quark::QuarkAppendFile *fx_read =
            Quark::QuarkAppendFile::NewNonEmptyReadFile(str1.c_str(), true);

        EXPECT_NE(fx_read, (void *)NULL);

        std::cout << "meta magic: " << fx_read->meta.magic << std::endl;
        std::cout << "bs:" << fx_read->meta.blksize << std::endl;
        std::cout << "bc:" << fx_read->meta.blkcount << std::endl;

        ret = fx_read->ReadAllBlocks();
        std::cout << "read result:" << ret << std::endl;
        size_t ix = 0;
        for (ByteBlockArray::iterator itr = fx_read->blk_memtable.begin();
             itr != fx_read->blk_memtable.end(); ++itr, ++ix) {
            bufx = (char *)(*itr)->GetCtn();
            std::cout << ix << ":" << bufx << std::endl;
        }

        delete fx_read;

        Quark::QuarkAppendFile::deinitialize();
    }

    static void tst_File_02() {
        int ret;
        char *bufx;
        std::string str1 = dirname + "file_02" + suffix;
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
        std::cout << "write1:" << opbuf1 << std::endl;        
        ret = fx->AppendBlockToBuffer(opbuf1);
        std::cout << "----------- Write ------------" << std::endl;
        std::cout << "Write(buffer):" << ret << std::endl;
        ret = fx->WriteAllBlocks();
        std::cout << "WriteFile:" << ret << std::endl;

        std::cout << "meta magic: " << fx->meta.magic << std::endl;
        std::cout << "bs:" << fx->meta.blksize << std::endl;
        std::cout << "bc:" << fx->meta.blkcount << std::endl;

        delete fx;
        
        std::cout << "----------- Write 2 ------------" << std::endl;
        Quark::QuarkAppendFile *fxx = Quark::QuarkAppendFile::NewNonEmptyAppendFile(str1.c_str());

        EXPECT_NE(fxx, (void *)NULL);
        std::cout << "meta magic: " << fxx->meta.magic << std::endl;
        std::cout << "bs:" << fxx->meta.blksize << std::endl;       
        std::cout << "bc:" << fxx->meta.blkcount << std::endl;      


        char *opbuf22 = (char *)fxx->AllocBlockBuffer();
        x = 'Y';
        for (size_t i = 0; i < sz && i < 10; i++) {
                opbuf22[i] = x;                         
                x++;                                   
        }
        std::cout << "write2:" << opbuf22 << std::endl;        
        ret = fxx->AppendBlockToBuffer(opbuf22);
        std::cout << "Write(buffer):" << ret << std::endl;
        ret = fxx->WriteAllBlocks();
        std::cout << "WriteFile:" << ret << std::endl;
        delete fxx;
        

        std::cout << "----------- Read ------------" << std::endl;
        Quark::QuarkAppendFile *fx_read =
            Quark::QuarkAppendFile::NewNonEmptyReadFile(str1.c_str(), true);

        EXPECT_NE(fx_read, (void *)NULL);

        std::cout << "meta magic: " << fx_read->meta.magic << std::endl;
        std::cout << "bs:" << fx_read->meta.blksize << std::endl;
        std::cout << "bc:" << fx_read->meta.blkcount << std::endl;

        ret = fx_read->ReadAllBlocks();
        std::cout << "read result:" << ret << std::endl;
        size_t ix = 0;
        for (ByteBlockArray::iterator itr = fx_read->blk_memtable.begin();
             itr != fx_read->blk_memtable.end(); ++itr, ++ix) {
            bufx = (char *)(*itr)->GetCtn();
            std::cout << ix << ":" << bufx << std::endl;
        }

        delete fx_read;


        Quark::QuarkAppendFile::deinitialize();
    }
    
    static void tst_File_03() {
        int ret;
        char *bufx;
        std::string str1 = dirname + "file_03" + suffix;
        Quark::QuarkAppendFile::initialize();
        Quark::QuarkAppendFile* f1 = Quark::QuarkAppendFile::NewEmptyAppendFile(str1.c_str());
        EXPECT_NE(f1, (void*)NULL);
        void *ptr1 = f1->AllocBlockBuffer();
        size_t sz = f1->GetBlockBufferSize();
        const char *x[]= { 
            "happy code<,dk, jrsa, fk.",
            "caffe latte, sandeli,nfff.(*&^$#"
        };
        std::cout << "write1: " << x[0] << "l:" << strlen(x[0]) << std::endl;
        memcpy(ptr1, x[0], strlen(x[0]));
        std::cout << "write before enc: " << (const char*)ptr1 << std::endl;
        f1->AppendBlockToBufferEnc(ptr1);
        std::cout << "write after enc: " << (const char*)ptr1 << std::endl;

        f1->WriteAllBlocks();
        delete f1;
        
        f1 = Quark::QuarkAppendFile::NewNonEmptyReadFile(str1.c_str(), true);
        f1->ReadAllBlocksDec();
        int bsz = f1->GetBlockCount();
        std::cout << "read1: " << std::endl;
        for(int i = 0; i < bsz; ++i) {
            std::cout << i << ": " << f1->GetBlock(i) << std::endl;
        }
        
        delete f1;
        Quark::QuarkAppendFile::deinitialize();
    }


    static void tst_encryto_01() {
        Quark::QEncryption qc;
        std::cout << qc.xkeys << std::endl;
        outputstr_by_int(qc.xkeys, XKEYSIZE);

        std::cout << qc.xivs << std::endl;
        outputstr_by_int(qc.xivs, XIVSIZE);
    }

    static void tst_encryto_02() {
        Quark::QEncryption qc;

        std::string test_str("abcABC I'm Big");
        std::string enc_text, dec_text;
        std::cout << "-------------org-----------------" << std::endl;
        std::cout << "-a:" << std::endl;
        std::cout << test_str << std::endl;
        std::cout << "-b:" << std::endl;
        outputstr_by_int(test_str);

        std::cout << "-------------Enc-----------------" << std::endl;
        qc.EncryptAndPut(enc_text, test_str);

        std::cout << "-a:" << std::endl;
        std::cout << enc_text << std::endl;

        std::cout << "-b:" << std::endl;
        outputstr_by_int(enc_text);
        
        std::cout << "-------------Dec-----------------" << std::endl;

        qc.DecryptAndGet(dec_text, enc_text);
        std::cout << "-a:" << std::endl;
        std::cout << dec_text << std::endl;
        std::cout << "-b:" << std::endl;
        outputstr_by_int(dec_text);
    }
    
    static void tst_encryto_03() {
        Quark::QEncryption qc;
        char test_str[128] = "abc*&^%%dddddhahahah!!!";
        char enc_str[128], dec_str[128];
        
        std::cout << "-------------org-----------------" << std::endl;
        std::cout << test_str << std::endl;
        
        std::cout << "-------------Enc-----------------" << std::endl;
        qc.EncryptAndPut(enc_str, test_str, 128);
        std::cout << enc_str << std::endl;
        
        
        std::cout << "-------------Dec-----------------" << std::endl;
        qc.DecryptAndGet(dec_str, enc_str, 128);
        std::cout << dec_str << std::endl;
    }


    static void outputstr_by_int(const std::string &str) {
        for (size_t i = 0; i < str.size(); ++i) {
            std::cout << (int)str[i] << " ";
        }
        std::cout << std::endl;
    }
    static void outputstr_by_int(const uchar_t* strx, size_t len) {
        for(size_t i = 0; i < len; i++){
            std::cout << (int)strx[i] << " ";
        }
        std::cout << std::endl;
    }

};
}  // namespace Quark

TEST(QAF, metablock1) { Quark::QAF_Test_Obj::tst_QFMetaBlock_mem_init(); }
TEST(QAF, metablock2) { Quark::QAF_Test_Obj::tst_QFMetaBlock_IO(); }
TEST(QAF, metablock3) { Quark::QAF_Test_Obj::tst_QAF_01(); }
TEST(QAFExternal, fileoperation) { Quark::QAF_Test_Obj::tst_File_01(); }
TEST(QAFExternal, fileoperation_append) { Quark::QAF_Test_Obj::tst_File_02(); }
TEST(QAFExternal, fileoperation_enc_dec) { Quark::QAF_Test_Obj::tst_File_03(); }

TEST(Encrypto, init) { Quark::QAF_Test_Obj::tst_encryto_01(); }

TEST(Encrypto, enc_dec) { Quark::QAF_Test_Obj::tst_encryto_02(); }
TEST(Encrypto, enc_dec2) { Quark::QAF_Test_Obj::tst_encryto_03(); }

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
