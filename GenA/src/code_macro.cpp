#include "code_macro.hpp"
#include <iostream>

// Constructor
code_macro::code_macro(size_t op_code, std::string operand_template, 
                       FuncPtr func)
    : op_code_(op_code)
    , operand_template_(operand_template)
    , funcPtr_(func) {
    if (op_code == 0 || operand_template.empty() || func == nullptr) {
        std::cerr << "Invalid arguments provided to constructor." << std::endl;
        op_code_ = 0;
        operand_template_ = "";
        funcPtr_ = nullptr;
    }
}

// Destructor
code_macro::~code_macro() {}

size_t code_macro::prog_data(std::string operand) {
    // First check if the operand matches the template
    if (is_valid_operand(operand)) {
        // call the pointer function to get the prog data.
        return funcPtr_(operand);
    }
    return NULL; 
}

bool is_valid_operand(std::string operand) {
}