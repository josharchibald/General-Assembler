// asm_line.cpp
// C++ file for teh assembly line class implementation.
// Revision History:
// 05/18/24 Joshua Archibald Initial revision.

// Included libraries.
#include <cstddef>
#include <tuple>
#include <functional>
#include "asm_line.hpp"
#include "isa.hpp"
#include <tuple>
#include <functional>
#include <code_macro.hpp>
#include <vector>
#include <string>
#include <unordered_map>



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

// When the line is asked to assemble itself it locates its own code macro and 
// looks to swap in any symbols in the arguments then sends it to the function.
size_t asm_line::assemble(isa cpu_isa, \
                          std::unordered_multimap<std::string, size_t> table,
                          size_t pc) {
    std::vector<std::string> args;   
    std::vector<std::string> symbolic_args;   
    size_t result;
    code_macro macro = cpu_isa.code_mac(op_name_, operand_);
    symbolic_args = macro.arguments;
    for (std::string symbol : symbolic_args) {
        if (symbol == PC) {
            args.insert(args.begin(), std::to_string(pc));
        }
        else if (table.find(symbol) != table.end()) {
        args.insert(args.begin(), std::to_string(table.find(symbol)->second));
        }
        else {
            args.insert(args.begin(), symbol);
        }
    }
    result = std::string::npos;
    try {
        code_macro::func_ptr function = macro.func();
        result = function(macro.op_code(), args);
    }
    catch (const std::exception& e) {
        return result;
    }
    return result;
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