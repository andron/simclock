# -*- mode:make; tab-width:2; -*-

include $(HEADER)

NAME     := simclock
VERSION  := 1.0.0
RELEASE  := 1
REQUIRES := # nothing
$(call setup)

_CXXFLAGS := -std=c++11 -march=native

TARGETS := simclock

include $(FOOTER)
