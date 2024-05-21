// asm_line.cpp
// C++ file for teh assembly line class implementation.
// Revision History:
// 05/18/24 Joshua Archibald Initial revision.

// Included libraries.
#include "asm_line.hpp"
#include "isa.hpp"
#include "code_macro.hpp"

// Constructor
asm_line::asm_line(std::string origin_file_path, std::string text, \
                 std::string label, std::string op_name, std::string operand) \
                    : origin_file_path_(origin_file_path), text_(text), \
                    label_(label), op_name_(op_name), operand_(operand) {}

// Destructor
asm_line::~asm_line() {}

// Public functions.
size_t asm_line::size(isa cpu_isa) {
    if (!operand_.empty()){
        code_macro macro = cpu_isa.code_mac(op_name_, operand_);
        return cpu_isa.code_mac(op_name_, operand_).num_inst_bits();
    }
    else {
        return 0;
    }
}


// Assessors.
std::string asm_line::origin_file(void) {
    return origin_file_path_;
}
std::string asm_line::text(void) {
    return text_;
}
std::string asm_line::label(void) {
    return label_;
}