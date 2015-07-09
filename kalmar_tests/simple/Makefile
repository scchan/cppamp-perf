CXX=/opt/kalmar/bin/clang++
CXXFLAGS = `/opt/kalmar/bin/clamp-config --cxxflags`
CXXFLAGS += -DUSE_RESTRICT
CXXFLAGS += -DUSE_ARRAY_VIEW


LDFLAGS = `/opt/kalmar/bin/clamp-config --ldflags`

SOURCES=simple.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=simple

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

