TARGET := test
TempSRCs := $(wildcard temp*.cc)
TempEXEs := $(basename $(TempSRCs))
CXX := g++
LDFLAGS := -lpthread

all:
$(TempEXEs): %:%.cc
	$(CXX) $^ -o $(TARGET) $(LDFLAGS)
help:
	@echo $(TempEXEs)

clean:
	rm -rf $(TARGET)
