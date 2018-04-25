CC := g++
libSRCs := QEncry.cc 
libchachaSRCs := poly1305.c chacha.c chachapoly_aead.c
mSRCs   := main.cc

libchachaSRCs := $(addprefix chacha20poly1305/, $(libchachaSRCs))
Iflag01 :=  -I./

all: test

libqe.so: $(libSRCs) $(libchachaSRCs)
	$(CC) $^ -shared -fPIC -o $@ $(Iflag01)

test: libqe.so
	$(CC) $(mSRCs) -o $@ -lqe -L.

clean:
	rm -f test
	rm -f libqe.so
