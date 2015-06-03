CXX=/opt/kalmar/bin/clang++
CXXFLAGS = `/opt/kalmar/bin/clamp-config --cxxflags`
CXXFLAGS += -g

LDFLAGS = `/opt/kalmar/bin/clamp-config --ldflags`
LDFLAGS += -g

SOURCES=convolution.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=convolution

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

