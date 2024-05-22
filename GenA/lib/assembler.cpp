// assembler.cpp
// C++ file for the assembler class implementation.
// Revision History:
// 05/18/24 Joshua Archibald Initial revision.
// 05/19/24 Joshua Archibald Implemented first_pass.
// 05/20/24 Joshua Archibald Debugged first_pass.
// 05/20/24 Joshua Archibald Pseudo operations implemented.
// 05/21/24 Joshua Archibald Pseudo operations debugged.
// 05/21/24 Joshua Archibald Implemented second_pass.


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
#include <cmath>


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
const size_t LABEL_DISPLAY_SIZE = 25;
const std::string LISTING_FILE_NAME = "list_gena.lst";
const std::string LINE_NUM = " line  number: ";

// Constructor.

// Constructor.

assembler::assembler(std::string entry_path, std::string isa_file_path, \
                     std::string output_file_path, bool verbose, \
                     bool list) : entry_path_(entry_path), \
                     cpu_isa_(isa_file_path), \
                     output_file_path_(output_file_path), \
                     verbose_(verbose), list_(list), pc_(0), data_used_(0) {}


// Destructor
assembler::~assembler() {};

// Public functions.
bool assembler::first_pass(void) {
    std::vector<std::pair<std::pair<std::string, size_t>, std::ifstream>> \
    asm_file_stack;
    std::ifstream entry_file;
    std::string line;
    size_t line_num = 0;;
    std::string temp_line;
    bool next_file;
    std::string file_path;
    size_t inst_size;
    bool success = true;
    
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
    asm_file_stack.push_back({{entry_path_, line_num}, std::move(entry_file)});

    // While there are still files to assemble, get the file name and file from
    // the top of the stack.
    while (!asm_file_stack.empty()) {
        std::pair<std::pair<std::string, size_t>, std::ifstream>& \
        asm_file_pair = asm_file_stack.back();
        std::ifstream& asm_file = asm_file_pair.second;

        next_file = false;
        // This is the line number stored with the file path
        line_num = asm_file_pair.first.second;
        file_path = asm_file_pair.first.first;

        while (std::getline(asm_file, line) && !next_file) {
            // Update the line number as it comes in and out of the stack.
            line_num++;
            asm_file_pair.first.second = line_num;
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
                    success = pseudo_op_handler(line, next_file, \
                                                asm_file_stack);
                }
                else {
                    // Make sure assembly line is valid before adding it to the 
                    // assembly program data member.
                    asm_line assembly_line = cpu_isa_.parse_asm(line, \
                    file_path);  
                    if (assembly_line.origin_file() != ASM_INVALID) {
                        if (assembly_line.label() != "") {
                            // Update the symbol table if there is a label and
                            // it is not the same name as any var or const.
                            if (symbol_table_.count(assembly_line.label()) == \
                                0) {
                                symbol_table_.insert({assembly_line.label(), \
                                                      pc_});
                            }
                            else  {
                                std::cerr << "Error: Redefinition of " << \
                                assembly_line.label() << " on line " << \
                                line_num << " in file " << file_path \
                                << std::endl;
                                success = false;
                            }
                        }
                        // If there is an instruction then put it at pc which
                        // maybe changed by code location.
                        inst_size = assembly_line.size(cpu_isa_);
                        if (inst_size > 0) {
                            auto it = asm_prog_.begin();
                            for (size_t i = 0; i < pc_; ++i) {
                                if (it != asm_prog_.end()) {
                                    ++it;
                                } else {
                                    asm_prog_.push_back(asm_line(ASM_INVALID, \
                                    ASM_INVALID, ASM_INVALID, ASM_INVALID, \
                                    ASM_INVALID));
                                    it = asm_prog_.end();
                                }
                            }
                            asm_prog_.insert(it, assembly_line);
                            pc_ += inst_size;
                        }
                        else {
                            // Still keep the line;
                            asm_prog_.push_back(assembly_line);
                        }
                    } 
                    else {
                        std::cerr << "Error: Invalid line of assembly on " << \
                        "line " << line_num << " in file " << \
                        file_path << std::endl;
                        success = false;
                    }
                }
            }
            if (pc_ > cpu_isa_.word_sizes().front() * \
                      cpu_isa_.mem_sizes().front()) {
                std::cerr << "Error: " << \
                std::to_string(cpu_isa_.mem_sizes().front()) << " words of " \
                << "the program memory exceeded on line " << line_num \
                << " in file " << file_path << std::endl;
                success = false;
            }
            if (cpu_isa_.harv_not_princ()) {
                if (data_used_ > cpu_isa_.word_sizes().back() * \
                        cpu_isa_.mem_sizes().back()) {
                    std::cerr << "Error: " << \
                    std::to_string(cpu_isa_.mem_sizes().back()) << " words " \
                    << "of the data memory exceeded on line " << line_num \
                    << " in file " << file_path << std::endl;
                    success = false;
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
    std::clog << "\nFirst pass complete. \n\nSymbol table:" \
              << std::endl;
    // Iterate through the symbol table and print each key-value pair.
    std::string disp_label;
    for (auto pair : symbol_table_) {
        disp_label = pair.first.substr(0, std::min(pair.first.size(), \
                     LABEL_DISPLAY_SIZE));
        std::clog << disp_label << \
        std::string(LABEL_DISPLAY_SIZE - disp_label.size(), ' ') << " | 0x" << \
        std::hex << pair.second << std::endl;
    }
    return success;
}

bool assembler::second_pass(void) {
    std::ofstream output_file(output_file_path_);
    std::ofstream list_file;

    if (!output_file) {
        std::cout << "Error: Cannot open output file " << output_file_path_ \
        << ". Will produce listing file " << LISTING_FILE_NAME << std::endl;
        list_ = true;
    }
    if (list_) {
        list_file.open(LISTING_FILE_NAME);
        list_file.clear();
    }

    size_t addr = 0;
    std::string value;
    size_t width = 5;
    size_t data;
    for (asm_line line : asm_prog_) {
        addr++;
        if (line.origin_file() != ASM_INVALID) {
            if (list_file) {
                if (line.size(cpu_isa_) > 0) {
                    data = line.assemble(cpu_isa_, symbol_table_, addr);
                    if (data != std::string::npos) {
                        list_file << std::setw(width) << std::setfill('0') \
                            << std::hex << addr << " " 
                            << std::setw(width) << std::setfill('0') << std::hex 
                            << line.assemble(cpu_isa_, symbol_table_, addr) 
                            << "\t;" 
                            << line.text() \
                            << std::endl;
                        addr += line.size(cpu_isa_);
                    }
                    else {
                        std::cerr << "Error: Function code macro" << std::endl;
                    }
                } 
                else {
                    list_file << "\t\t\t\t;" << line.text() \
                    << std::endl;
                }
            }
        }
    }
    if (output_file) {
        output_file.close();
    }
    if (list_file) {
        list_file.close();
    }
    return true;
}



// Helper functions.


bool assembler::pseudo_op_handler(std::string line, bool& next_file, \
        std::vector<std::pair<std::pair<std::string, size_t>, std::ifstream>>& \
                       asm_file_stack) {
    std::vector<std::string> line_data;
    std::string file_path = asm_file_stack.back().first.first;
    size_t line_num = asm_file_stack.back().first.second;
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
                std::cerr << "Error: Invalid code location entry: " << \
                line_data.at(CODE_LOC_SIZE) << " on line " << line_num << \
                " in file: " << file_path << std::endl;
                return false;
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
                std::cerr << "Error: Invalid variable word count entry " << \
                line_data.at(VAR_DEC_SIZE) << " on line " << line_num << \
                " in file: " << file_path << std::endl;
                return false;
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
                file_path << std::endl;
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
                std::cerr << "Error: Invalid constant definition entry: " << \
                line_data.at(CONST_SIZE) << " on line " << line_num << \
                " in file: " << file_path << std::endl;
                return false;
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
                file_path << std::endl;
                return false;
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
                asm_file_stack.push_back({{new_file_path, 0}, std::move(new_file)});
                asm_file_paths_.insert(new_file_path);
                next_file = true;
            }
        }
    }
    return true;
}
