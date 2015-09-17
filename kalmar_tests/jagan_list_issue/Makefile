OPENCL_INC=/opt/AMDAPPSDK-3.0-0-Beta/include
PREFIX=/opt/kalmar
CLANG_AMP=$(PREFIX)/bin/clang++
CLAMP_CONFIG=$(PREFIX)/bin/clamp-config 
CLAMPINC=`$(CLAMP_CONFIG) --install --cxxflags`
CLAMPLIB=`$(CLAMP_CONFIG) --install --ldflags`
SHARED_CXXFLAGS=$(CLAMPINC) -I./include -I$(OPENCL_INC)  -Xclang -fhsa-ext -Xlinker --disable-opencl

CORE_FILES = list
ORIG_SRCS = $(addsuffix .cpp,$(CORE_FILES))
SRCS = $(ORIG_SRCS)
OBJS = $(addprefix $(OPATH),$(addsuffix $(OEXT),$(CORE_FILES)))
OUTNAME = kalmar_list_test.out
OUTFILE = ./$(OUTNAME)
OUTCMD = -o $(OUTFILE)

all: $(SRCS)
	$(CLANG_AMP) $(SHARED_CXXFLAGS) $(CLAMPLIB) $(CFLAGS) $(XCFLAGS) $(SRCS) $(OUTCMD)

.PHONY: clean
clean:
	rm -f $(OUTFILE)
