tst_src := $(wildcard tst-*.cc)
tst_exe := $(basename $(tst_src))

all: $(tst_exe) install
	@echo "--EXEs: "$(tst_exe)

# each tst-... file will make an executable
$(tst_exe): %:%.cc
	@echo $@ $^
	$(CXX) $(CXXFLAGS) $(Common_INCDIR_CXXFLAGS) $(Common_LDDIR_CXXFLAGS) $^ -o $@ $(Common_LIB_LDFLAGS) $(Common_Deps_LDFLAGS)

install: $(tst_exe)
	cp -f $(tst_exe) $(gCommonOUT)

clean:
	$(RM) $(tst_exe)
	$(RM) *.o
