CC := g++


libSRCs := QEncry.cc 
#libchachaSRCs := poly1305.c chacha.c chachapoly_aead.c
mSRCs   := main.cc

Iflag01 :=  -I./


Iflag02 :=  -I./3rdparty/googletest-release-1.8.0/googletest/include/
LDflag02 :=  -L./3rdparty/googletest-release-1.8.0/build/googlemock/gtest/ -lgtest

all: test

libqe.so: $(libSRCs)
	$(CC) $^ -shared -fPIC -o $@ $(Iflag01)

test: libqe.so gtest
	$(CC) $(mSRCs) -o $@ -lqe -L. $(Iflag02) $(LDflag02)

gtest: 
	@echo "---------make gtest--------" 


clean:
	rm -f test
	rm -f libqe.so
