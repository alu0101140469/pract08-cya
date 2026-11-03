CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2

TARGET := dfa2grammar
SOURCES := main.cc automata.cc grammar.cc state.cc
OBJECTS := $(SOURCES:.cc=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cc automata.h grammar.h state.h
automata.o: automata.cc automata.h grammar.h state.h
grammar.o: grammar.cc grammar.h
state.o: state.cc state.h

clean:
	rm -f $(OBJECTS) $(TARGET)