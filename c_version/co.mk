
##### Makefile Rules ##########
MAIL_ROOT=.
SRCROOT=.

##define the compliers
CPP = $(CXX)
AR = ar -rc
RANLIB = ranlib

CPPSHARE = $(CPP) -fPIC -shared -O2 -pipe -L$(SRCROOT)/solib/ -o 
CSHARE = $(CC) -fPIC -shared -O2 -pipe -L$(SRCROOT)/solib/ -o 

ifeq ($v,release)
CFLAGS= -O2 $(INCLS) -fPIC  -DLINUX -pipe -Wno-deprecated -c
else
CFLAGS= -g $(INCLS) -fPIC -DLINUX -pipe -c -fno-inline
endif

ifneq ($v,release)
BFLAGS= -g
endif

STATICLIBPATH=$(SRCROOT)/lib
DYNAMICLIBPATH=$(SRCROOT)/solib

INCLS += -I$(SRCROOT)

## default links
ifeq ($(LINKS_DYNAMIC), 1)
LINKS += -L$(DYNAMICLIBPATH) -L$(STATICLIBPATH)
else
LINKS += -L$(STATICLIBPATH)
endif

CPPSRCS  = $(wildcard *.cpp)
CSRCS  = $(wildcard *.c)
CPPOBJS  = $(patsubst %.cpp,%.o,$(CPPSRCS))
COBJS  = $(patsubst %.c,%.o,$(CSRCS))

SRCS = $(CPPSRCS) $(CSRCS)
OBJS = $(CPPOBJS) $(COBJS)

CPPCOMPI=$(CPP) $(CFLAGS) -Wno-deprecated
CCCOMPI=$(CC) $(CFLAGS)

BUILDEXE = $(CPP) $(BFLAGS) -o $@ $^ $(LINKS) 
CLEAN = rm -f *.o 

CPPCOMPILE = $(CPPCOMPI) $< $(FLAGS) $(INCLS) $(MTOOL_INCL) -o $@
CCCOMPILE = $(CCCOMPI) $< $(FLAGS) $(INCLS) $(MTOOL_INCL) -o $@

ARSTATICLIB = $(AR) $@.tmp $^ $(AR_FLAGS); \
			  if [ $$? -ne 0 ]; then exit 1; fi; \
			  test -d $(STATICLIBPATH) || mkdir -p $(STATICLIBPATH); \
			  mv -f $@.tmp $(STATICLIBPATH)/$@;

BUILDSHARELIB = $(CPPSHARE) $@.tmp $^ $(BS_FLAGS); \
				if [ $$? -ne 0 ]; then exit 1; fi; \
				test -d $(DYNAMICLIBPATH) || mkdir -p $(DYNAMICLIBPATH); \
				mv -f $@.tmp $(DYNAMICLIBPATH)/$@;

.cpp.o:
	$(CPPCOMPILE)
.c.o:
	$(CCCOMPILE)
