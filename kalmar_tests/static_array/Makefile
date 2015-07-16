CXX=/opt/kalmar/bin/clang++
CXXFLAGS = `/opt/kalmar/bin/clamp-config --cxxflags`
CXXFLAGS += -Xclang -fhsa-ext

LDFLAGS = `/opt/kalmar/bin/clamp-config --ldflags`

SOURCES=static_array.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=static_array

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

