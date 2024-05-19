// code_macro.cpp
// C++ file for code macro class implementation.
// Revision History:
// 05/18/24 Joshua Archibald Initial revision.


// Included libraries.
#include "code_macro.hpp"
#include <stdlib.h>
#include <string>
#include <iostream>

// Constants.


// Constructor.
code_macro::code_macro(size_t op_code, \
                       std::vector<std::string> operand_template, \
                       func_ptr func, size_t num_inst_bits) : \
                       op_code_(op_code), operand_template_(operand_template), \
                       func_(func), num_inst_bits_(num_inst_bits) {};

// Destructor
code_macro::~code_macro() {};

// Public functions.
// size_t translate(std::string op_name, std::string operand){}

// Helper functions.
