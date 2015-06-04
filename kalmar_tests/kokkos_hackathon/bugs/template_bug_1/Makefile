CXX=/opt/kalmar/bin/clang++
CXXFLAGS = `/opt/kalmar/bin/clamp-config --cxxflags`
LDFLAGS = `/opt/kalmar/bin/clamp-config --ldflags`

SOURCES=test_template.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=test_template

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

