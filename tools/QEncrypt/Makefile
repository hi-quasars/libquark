CC := g++
C := gcc
MAKE := make


libSRCs := QEncry.cc 
clibSRCs := libqencrypt.cc

tSRCs   := tests/tst-block.cc

Iflag01 :=  -I./

cryptopp_dir := ./3rdparty/cryptopp/cryptopp-CRYPTOPP_6_0_0
gtest_dir := ./3rdparty/googletest-release-1.6.0/build/
cinclude_dir := ./cinclude/
ctestsrc_dir := ./ctest/

Iflag02 :=  -I./3rdparty/googletest-release-1.6.0/include/

Iflag03 := -I$(cryptopp_dir) 
Iflag04 := -I$(cinclude_dir)

LDflag02 :=  -L$(gtest_dir) -lgtest -lgtest_main \
		-lpthread
LDflag03 := -L$(cryptopp_dir)/ -lcryptopp
						 

all: test

libqe.so: cryptopp
	$(CC) $(libSRCs) -shared -fPIC -o $@ $(Iflag01) $(Iflag03) $(LDflag03)

libcqenc.so: cryptopp
	$(CC) $(libSRCs) $(clibSRCs) -shared -fPIC -o $@ $(Iflag01) $(Iflag03) $(Iflag04) $(LDflag03)

CTEST: libcqenc.so
	cp libcqenc.so $(ctestsrc_dir)
	$(MAKE) -C $(ctestsrc_dir)

test: libqe.so gtest
	$(CC) $(tSRCs) -o $@ -lqe -L. $(Iflag01) $(Iflag02) $(LDflag02) $(Iflag03) $(LDflag03)
gtest:
	@echo "---------make libgtest--------" 
	$(MAKE) -C $(gtest_dir)

gtest-force-rebuild: 
	@echo "---------make libgtest--------" 
	cd $(gtest_dir)/..; rm -rf build; mkdir build; cd build; cmake ..;
	$(MAKE) -C $(gtest_dir)

cryptopp:
	@echo "---------make libcryptopp-----"
	$(MAKE) -C $(cryptopp_dir)
cryptopp_test:
	$(CC) enc_test.cc -o $@ $(Iflag03) $(LDflag03)

clean:
	rm -f test
	rm -f libqe.so libcqenc.so
	rm -f tests/*tst.txt
	$(MAKE) -C $(ctestsrc_dir) clean
