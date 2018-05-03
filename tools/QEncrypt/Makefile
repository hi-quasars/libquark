CC := g++


libSRCs := QEncry.cc 
mSRCs   := main.cc

tSRCs   := tests/tst-block.cc

Iflag01 :=  -I./

Iflag02 :=  -I./3rdparty/googletest-release-1.6.0/include/
LDflag02 :=  -L./3rdparty/googletest-release-1.6.0/build/ -lgtest -lgtest_main \
		-lpthread
			 #-L./3rdparty/googletest-release-1.6.0/build/googlemock/ -lgmock -lgmock_main \


all: test

libqe.so: $(libSRCs)
	$(CC) $^ -shared -fPIC -o $@ $(Iflag01)

test: libqe.so gtest
	$(CC) $(tSRCs) -o $@ -lqe -L. $(Iflag01) $(Iflag02) $(LDflag02)

gtest: 
	@echo "---------make gtest--------" 


clean:
	rm -f test
	rm -f libqe.so
	rm -f tests/*tst.txt
