// assembler.cpp
// C++ file for the assembler class implementation.
// Revision History:
// 05/18/24 Joshua Archibald Initial revision.
// 05/19/24 Joshua Archibald Implemented first_pass.
// 05/20/24 Joshua Archibald Debugged first_pass.
// 05/20/24 Joshua Archibald Pseudo operations implemented.


// Included libraries.
#include "assembler.hpp"
#include "asm_line.hpp"
#include "isa.hpp"
#include <stdlib.h>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <iomanip>
#include <unordered_set>

// Constants.
const std::string PSEUDO_OP = ".";
const std::string CONTINUE = "\\";
const std::string CODE_LOC = "org";
const size_t CODE_LOC_SIZE = 2;
const std::string VAR_DEC = "data";
const size_t VAR_DEC_SIZE = 3;
const std::string CONST = "def";
const size_t CONST_SIZE = 2;
const std::string INCLUDE = "include";
const size_t INCLUDE_SIZE = 2;

// Constructor.

assembler::assembler(std::string entry_path, std::string isa_file_path, \
                     std::string output_folder_path, bool list) : \
                    entry_path_(entry_path), cpu_isa_(isa_file_path), \
                    output_folder_path_(output_folder_path), list_(list), \
                    pc_(0), data_used_(0) {}


// Destructor
assembler::~assembler() {};

// Public functions.
void assembler::first_pass(void) {
    std::list<std::pair<std::string, std::ifstream>> asm_file_stack;
    std::ifstream entry_file;
    std::string line;
    size_t line_num;
    std::string temp_line;
    bool next_file;
    
    // Set the valid assembly extension to be the extension of the entry point.
    valid_extension_ = entry_path_.substr(entry_path_.find_last_of('.'));

    // Push the entry file onto the file stack.
    entry_file.open(entry_path_);
    // Display error message and exit if file can not be opened.
    if (!entry_file) {
        std::cerr << "Error: Cannot open entry file: " << entry_path_ << \
        std::endl;
        exit(EXIT_FAILURE);
    }
    asm_file_stack.push_back({entry_path_, std::move(entry_file)});

    // While there are still files to assemble, get the file name and file from
    // the top of the stack.
    while (!asm_file_stack.empty()) {
        std::pair<std::string, std::ifstream>& asm_file_pair = \
                                               asm_file_stack.back();
        std::ifstream& asm_file = asm_file_pair.second;

        next_file = false;
        line_num = 0;

        while (std::getline(asm_file, line) && !next_file) {
            line_num++;
            // If while parsing, the continue symbol is at the end of the line,
            // save the line in a temp for when the next cycle comes.
            if (line.find(CONTINUE) == (line.size() - 1)) {
                temp_line += line.substr(0, line.size() - 2);
            }
            else {
                // No line continuations so line is updated and temp line is 
                // reset.
                line = temp_line + line;
                temp_line = "";
                // If the line is a pseudo operation, pass it to the handler, 
                // which can modify the file and line search.
                if ((line.find(PSEUDO_OP) == 0)) {
                    pseudo_op_handler(line, next_file, asm_file_stack, \
                                      line_num);
                }
                else {
                    // Make sure assembly line is valid before adding it to the 
                    // assembly program data member.
                    asm_line assembly_line = cpu_isa_.parse_asm(line, \
                    asm_file_pair.first);  
                    if (assembly_line.origin_file() != ASM_INVALID) {
                        auto it = asm_prog_.begin();
                        std::advance(it, pc_);
                        asm_prog_.insert(it, assembly_line);
                        if (assembly_line.label() != "") {
                            // Update the symbol table if there is a label and
                            // it is not the same name as any var or const.
                            if (symbol_table_.count(assembly_line.label()) == \
                                0) {
                                symbol_table_.insert({assembly_line.label(), \
                                                      pc_});
                            }
                            else  {
                                std::cerr << "Error: Redefinition of  " << \
                                assembly_line.label() << " on line " << \
                                line_num << " in file " << asm_file_pair.first \
                                << std::endl;
                            }
                        }
                        else {
                            // Update the program counter with the previous line 
                            // size.
                            pc_ += assembly_line.size(cpu_isa_);
                        }
                    } 
                    else {
                        std::cerr << "Error: Invalid line of assembly on " << \
                        "line " << line_num << " in file " << \
                        asm_file_pair.first << std::endl;
                    }
                }
            }
        }
        // If the end of the file is reached, close the file and pop it off the
        // stack.
        if (!next_file) {
            asm_file.close();
            asm_file_stack.pop_back();
        }        
        // If next file is set true, the next file on the stack is opened.
    }
    // Iterate through the unordered_multimap and print each key-value pair
    for (const auto& pair : symbol_table_) {
        std::cout << "|" << pair.first << "|" << pair.second << std::endl;
    }
}

// Helper functions.
void assembler::pseudo_op_handler(std::string line, bool& next_file, \
                       std::list<std::pair<std::string, std::ifstream>>& \
                       asm_file_stack, size_t line_num) {
    std::vector<std::string> line_data;
    line_data = cpu_isa_.split_by_spaces(line.substr(PSEUDO_OP.length()));
    // Conditionals for pseudo operations as they are all very different.
    // The number after the code location pseudo op gets set to the pc. 
    if (cpu_isa_.strip_and_lower(line_data.at(0)) == CODE_LOC) {
        if (line_data.size() == CODE_LOC_SIZE) {
            try {
				std::stoul(line_data.at(CODE_LOC_SIZE - 1));
            }
            // Display error message if string is not a positive integer.
            catch (const std::exception& e) {
                std::cout << "Error: Invalid code location entry: " << \
                line_data.at(CODE_LOC_SIZE) << " on line " << line_num << \
                " in file: " << asm_file_stack.back().first << std::endl;
                return;
            }
            pc_ = std::stoul(line_data.at(CODE_LOC_SIZE - 1)) * \
                  cpu_isa_.word_sizes().front();
        }
    }
    else if (cpu_isa_.strip_and_lower(line_data.at(0)) == VAR_DEC) {
        std::string var_name;
        size_t *memory = &pc_;
        size_t bit_num = cpu_isa_.word_sizes().front();
        // The string after the variable declaration pseudo operation is put
        // into the symbol table with the data used pointer if harvard but with 
        // the pc if prenotion then the pointer is moved up by the number after 
        // that multiplied by the word size.
        if (line_data.size() == VAR_DEC_SIZE) {
            try {
				std::stoul(line_data.at(VAR_DEC_SIZE - 1));
                var_name = line_data.at(VAR_DEC_SIZE - 2);
            }
            // Display error message if string is not a positive integer.
            catch (const std::exception& e) {
                std::cout << "Error: Invalid variable word count entry: " << \
                line_data.at(VAR_DEC_SIZE) << " on line " << line_num << \
                " in file: " << asm_file_stack.back().first << std::endl;
                return;
            }
            if (cpu_isa_.harv_not_princ()) {
                memory = &data_used_;
                bit_num = cpu_isa_.word_sizes().back();
            }
            // If the var name already exists as a variable or label display an
            // error.
            if (symbol_table_.count(var_name) == \
                0) {
                symbol_table_.insert({var_name, \
                                    *memory});
            }
            else  {
                std::cerr << "Error: Redefinition of " << var_name << \
                " on line " << line_num << " in file " << \
                asm_file_stack.back().first << std::endl;
            }           
            // The updated memory space pointer is updated.
            *memory = *memory + \
                      (bit_num * std::stoul(line_data.at(VAR_DEC_SIZE - 1)));
        }
    }
    else if (cpu_isa_.strip_and_lower(line_data.at(0)) == CONST) {
        std::string const_name;
        // The string after the constant declaration pseudo operation is put
        // into the symbol table with its value.
        if (line_data.size() == CONST_SIZE) {
            try {
				std::stoul(line_data.at(CONST_SIZE - 1));
                const_name = std::stoul(line_data.at(CONST_SIZE - 2));
            }
            // Display error message if string is not a positive integer.
            catch (const std::exception& e) {
                std::cout << "Error: Invalid constant definition entry: " << \
                line_data.at(CONST_SIZE) << " on line " << line_num << \
                " in file: " << asm_file_stack.back().first << std::endl;
                return;
            }
            // If the const name already exists as a variable or label or const
            // display an error.
            if (symbol_table_.count(const_name) == \
                0) {
                symbol_table_.insert({const_name, \
                                   std::stoul(line_data.at(CONST_SIZE - 1))});
            }
            else  {
                std::cerr << "Error: Redefinition of " << const_name << \
                " on line " << line_num << " in file " << \
                asm_file_stack.back().first << std::endl;
            }           
        }
    }
    else if (cpu_isa_.strip_and_lower(line_data.at(0)) == INCLUDE) {
        if (line_data.size() == INCLUDE_SIZE) {
            std::string new_file_path = line_data.at(INCLUDE_SIZE - 1);
            std::ifstream new_file(new_file_path);
            bool add_file = true;
            // If an included file cannot be opened, has already been included,
            // or does not have the right extension, display an error message 
            // and don't add the file.
            if (!new_file) {
                std::cerr << "Error: Unable to open file: " << new_file_path \
                          << ". File not included." << std::endl;
                add_file = false;
            }
            if (asm_file_paths_.find(new_file_path) != asm_file_paths_.end()) {
                std::cerr << "Error: File: " << new_file_path << " already " \
                          << "included. File skipped." << std::endl;
                add_file = false;
            }
            if (new_file_path.substr(new_file_path.find_last_of('.')) !=
                valid_extension_) {
                std::cerr << "Error: File: " << new_file_path << " does not " \
                          << "have valid extension: " << valid_extension_ << \
                          ". File skipped." << std::endl;
                add_file = false;
            }
            // Indicate that the next file on the stack should be moved to and 
            // add the included file.
            if (add_file) {
                asm_file_stack.push_back({new_file_path, std::move(new_file)});
                asm_file_paths_.insert(new_file_path);
                next_file = true;
            }
        }
    }
}