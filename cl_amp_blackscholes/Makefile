
CC=g++

CPPFLAGS+=-I$(AMDAPPSDKROOT)/include 
LDFLAGS+=-L$(AMDAPPSDKROOT)/lib/x86_64 -lOpenCL

CPPFLAGS+=-I/home/xkerox/Code/P4_PAE/main/sw/AEDS/AppEng/personal/scchan/utilities/include


CPPFLAGS+=-g
CPPFLAGS+=-O3

CPPFLAGS+=-DENABLE_OPENCL

#CPPFLAGS+=-DENABLE_CODEXL -I/opt/AMD/CodeXL/AMDTActivityLogger/include
#LDFLAGS+=-L/opt/AMD/CodeXL/AMDTActivityLogger/bin/x86_64 -lAMDTActivityLogger
#LDFLAGS+=-L/opt/AMD/CodeXL  -lAMDTBaseTools

all: bs_opencl

bs_opencl: bs_opencl.o
	$(CC) $^ $(LDFLAGS) -o $@

clean:

