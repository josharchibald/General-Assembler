// assembler.cpp
// C++ file for assembler class implementation.
// Revision History:
// 05/18/24 Joshua Archibald Initial revision.


// Included libraries.
#include "assembler.hpp"
#include <stdlib.h>
#include <string>
#include <iostream>

// Constants.


// Constructor.
assembler::assembler(std::string entry_path, std::string isa_file_path) : \
                    entry_path_(entry_path), cpu_isa_(isa_file_path) {}

// Destructor
assembler::~assembler() {};

// Public functions.
// size_t translate(std::string op_name, std::string operand){}

// Helper functions.
