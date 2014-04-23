
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

all: bs_opencl bs_amp

bs_opencl: bs_opencl.o
	$(CC) $^ $(LDFLAGS) -o $@


bs_amp: bs_opencl.cpp
	../script/buildme_mod bs_opencl.cpp -I../utilities/include -DENABLE_CPPAMP;mv bs_opencl.cpp.out bs_amp

clean:
	rm -f bs_opencl bs_amp kernel.* *.o *.ll
