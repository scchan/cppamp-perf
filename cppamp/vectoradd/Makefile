CXX=/opt/kalmar/bin/clang++
CXXFLAGS = `/opt/kalmar/bin/clamp-config --cxxflags`
LDFLAGS = `/opt/kalmar/bin/clamp-config --ldflags`

SOURCES=vectoradd.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=vectoradd

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

