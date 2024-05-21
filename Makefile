# 
# Makefile for GenA Program
#

CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -Wno-long-long -Wno-variadic-macros -fexceptions -std=c++17 -g

# Define NDEBUG in release build
ifndef DEBUG
CXXFLAGS+=-DNDEBUG
endif

# Base directory
BASEDIR=/home/joshua/General-Assembler/GenA

INCLUDES=-I$(BASEDIR)/include

# Automatically list all .cpp files in src and lib directories within the base directory
SOURCES=$(wildcard $(BASEDIR)/src/*.cpp $(BASEDIR)/lib/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=gena

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@
	sudo mv $@ /usr/local/bin/
	rm -f $(OBJECTS)

# General rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Adjust pattern rule to include the base directory
$(BASEDIR)/%.o: $(BASEDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	sudo rm -f /usr/local/bin/$(EXECUTABLE)

	sudo rm -f /usr/local/bin/$(EXECUTABLE)

