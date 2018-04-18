# global configs

ConfName := configs.make
include ../configs.make

gCommonROOT := $(shell pwd)
gCommonOUT := $(gCommonROOT)/output
gCommonINC := $(gCommonROOT)/include

CXX := $(gGXX)
AR := ar rcs
RM := rm -rf
MKDIR := mkdir -p
CXXFLAGS := -O2 -std=c++11

Common_Deps_LDFLAGS := -lpthread

Common_INCDIR_CXXFLAGS := -I$(gCommonINC) -I$(gCommonROOT)
Common_LDDIR_CXXFLAGS := -L$(gCommonOUT)
Common_LIB_LDFLAGS  := -lcommon

TESTPATH := test

common_src := $(wildcard *.cc)
common_obj := $(patsubst %.cc, %.o, $(common_src))
target_a := libcommon.a

export CXX
export RM
export gCommonINC
export gCommonOUT
export Common_Deps_LDFLAGS
export Common_INCDIR_CXXFLAGS
export Common_LDDIR_CXXFLAGS
export Common_LIB_LDFLAGS

common_all: env $(target_a)
	cp $(target_a) $(gCommonOUT)

$(target_a): $(common_obj)
	@echo "----[libcommon]----"
	$(AR) $@ $^

$(common_obj) : %.o : %.cc
	$(CXX) -c $(CXXFLAGS) $(Common_INCDIR_CXXFLAGS) $< -o $@

env:
	@echo "--CXX:  "$(CXX)
	@if [ ! -d $(gCommonOUT) ]; then 	\
		$(MKDIR) $(gCommonOUT); 		\
	fi
	
test: common_all
	make -C $(TESTPATH)

help:
	@echo "Targets: $(target_a) test"

.PHONY: clean
clean:
	@echo "----[clean]----"
	$(RM) *.o
	$(RM) *.a
	make -C $(TESTPATH) clean
	$(RM) $(gCommonOUT)
