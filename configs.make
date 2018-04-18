ConfName := configs.make
CCROOT := $(dir $(lastword $(MAKEFILE_LIST)))

include $(CCROOT)/deps/$(ConfName)
export gGXX := $(realpath $(CCROOT)/deps/$(GXX_bin_5_3))
