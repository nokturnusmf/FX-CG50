.SUFFIXES:

ifeq ($(strip $(FXCGSDK)),)
export FXCGSDK := $(abspath ../../)
endif

DISPLAYNAME	:= Polydiv

CFLAGS		= $(GCFLAGS)
CXXFLAGS	= $(GCXXFLAGS)
LDFLAGS		= $(GLDFLAGS)

LIBS		= $(GLIBS)

INCLUDES	:=
LIBDIRS		:=

include ../common/project
include ../common/prizm_rules
