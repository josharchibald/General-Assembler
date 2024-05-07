# 
# Makefile for GenA Program
#

CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -Wno-long-long -Wno-variadic-macros -fexceptions -std=c++17

# Define NDEBUG in release build
ifndef DEBUG
CXXFLAGS+=-DNDEBUG
endif

INCLUDES=-Iinclude

# Automatically list all .cpp files in src and lib directories
SOURCES=$(wildcard src/*.cpp lib/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=gena

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@
	sudo mv $@ /usr/local/bin/

# General rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	sudo rm -f /usr/local/bin/$(EXECUTABLE)

