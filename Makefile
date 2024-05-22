# Makefile for GenA Program

CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -Wno-long-long -Wno-variadic-macros -fexceptions -std=c++17 -g

# Define NDEBUG in release build
ifndef DEBUG
CXXFLAGS+=-DNDEBUG
endif

# Base directory
BASEDIR=GenA

INCLUDES=-I$(BASEDIR)/include

SOURCES=$(wildcard $(BASEDIR)/src/*.cpp $(BASEDIR)/lib/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=gena

# Define installation path
INSTALL_PATH_LINUX=/usr/local/bin
INSTALL_PATH_WINDOWS=$(BASEDIR)\bin

# Determine OS
ifeq ($(OS),Windows_NT)
    RM=del /Q
    INSTALL_PATH=$(INSTALL_PATH_WINDOWS)
else
    RM=rm -f
    INSTALL_PATH=$(INSTALL_PATH_LINUX)
    INSTALL_CMD=sudo mv $@ $(INSTALL_PATH)/
    UNINSTALL_CMD=sudo rm -f $(INSTALL_PATH)/$(EXECUTABLE)
endif

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@
ifeq ($(OS),Windows_NT)
	mkdir $(INSTALL_PATH) || true
	move $@ $(INSTALL_PATH)/
else
	$(INSTALL_CMD)
endif
	$(RM) $(OBJECTS)

# General rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Adjust pattern rule to include the base directory
$(BASEDIR)/%.o: $(BASEDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJECTS)
ifeq ($(OS),Windows_NT)
	$(RM) $(INSTALL_PATH)\$(EXECUTABLE)
else
	$(UNINSTALL_CMD)
endif
