CC := g++
MAKE := make


libSRCs := QEncry.cc 
mSRCs   := main.cc

tSRCs   := tests/tst-block.cc

Iflag01 :=  -I./

cryptopp_dir := ./3rdparty/cryptopp/cryptopp-CRYPTOPP_6_0_0
gtest_dir := ./3rdparty/googletest-release-1.6.0/build/

Iflag02 :=  -I./3rdparty/googletest-release-1.6.0/include/

Iflag03 := -I$(cryptopp_dir) 
LDflag02 :=  -L$(gtest_dir) -lgtest -lgtest_main \
		-lpthread
LDflag03 := -L$(cryptopp_dir)/ -lcryptopp
								 

all: test

libqe.so: cryptopp
	$(CC) $(libSRCs) -shared -fPIC -o $@ $(Iflag01) $(Iflag03) $(LDflag03)

test: libqe.so gtest
	$(CC) $(tSRCs) -o $@ -lqe -L. $(Iflag01) $(Iflag02) $(LDflag02)
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

clean:
	rm -f test
	rm -f libqe.so
	rm -f tests/*tst.txt
