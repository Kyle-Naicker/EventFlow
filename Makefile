CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -g
TARGET   := eventflow

SOURCES  := $(wildcard *.cpp)
OBJECTS  := $(patsubst %.cpp,%.o,$(SOURCES))

.PHONY: all clean docs valgrind

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

docs:
  doxygen -g
  nano Doxyfile
	doxygen Doxyfile

clean:
	rm -f $(OBJECTS) $(TARGET)
