CXX=/opt/kalmar/bin/clang++
CXXFLAGS = `/opt/kalmar/bin/clamp-config --cxxflags`
CXXFLAGS += -DUSE_RESTRICT
CXXFLAGS += -DUSE_ARRAY_VIEW
CXXFLAGS += -Xclang -fhsa-ext


LDFLAGS = `/opt/kalmar/bin/clamp-config --ldflags`

SOURCES=group_mem.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=group_mem

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

