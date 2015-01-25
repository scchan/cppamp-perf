
CC=g++

CPPFLAGS+=-I$(AMDAPPSDKROOT)/include 
LDFLAGS+=-L$(AMDAPPSDKROOT)/lib/x86_64 -lOpenCL

UTIL_INCLUDE=-I../utilities/include 
UTIL_LIB=-L../utilities/bin
UTIL_LIB+=-lStackTimer

CPPFLAGS+=$(UTIL_INCLUDE)
LDFLAGS+=$(UTIL_LIB)

CPPFLAGS+=-g
CPPFLAGS+=-O3

CPPFLAGS+=-DENABLE_OPENCL

# CodeXL support
CODEXL_PATH=/opt/AMD/CodeXL
CODEXL_CPPFLAGS=-DENABLE_CODEXL -I$(CODEXL_PATH)/AMDTActivityLogger/include
CODEXL_LDFLAGS=-L$(CODEXL_PATH)/AMDTActivityLogger/bin/x86_64 -lAMDTActivityLogger
CODEXL_LDFLAGS+=-L$(CODEXL_PATH)  -lAMDTBaseTools

#CPPFLAGS+=$(CODEXL_CPPFLAGS)
#LDFLAGS+=$(CODEXL_LDFLAGS)

all: memtest_opencl memtest_amp

memtest_opencl: memtest.o
	$(CC) $^ $(LDFLAGS) -o $@

memtest_amp: memtest.cpp
	clang++ `clamp-config --install --cxxflags --ldflags` -Xclang -fhsa-ext memtest.cpp -I../utilities/include -DENABLE_CPPAMP $(UTIL_INCLUDE) $(UTIL_LIB) -o memtest_amp

clean:
	rm -f memtest_opencl memtest_amp kernel.* *.o *.ll
