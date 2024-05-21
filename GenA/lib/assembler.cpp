// assembler.cpp
// C++ file for the assembler class implementation.
// Revision History:
// 05/18/24 Joshua Archibald Initial revision.
// 05/19/24 Joshua Archibald First pass implemented.


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

// Constants.
const std::string PSEUDO_OP = ".";
const std::string CONTINUE = "\\";

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
    std::string valid_extension;
    std::list<std::pair<std::string, std::ifstream>> asm_file_stack;
    std::ifstream entry_file;
    std::string line;
    size_t line_num;
    std::string temp_line;
    bool next_file;
    
    // Set the valid assembly extension to be the extension of the entry point.
    valid_extension = entry_path_.substr(entry_path_.find_last_of('.'), \
                                         entry_path_.length() - 1);

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
                    pseudo_op_handler(line, next_file, asm_file_stack);
                }
                else {
                    // Make sure assembly line is valid before adding it to the 
                    // assembly program data member.
                    asm_line assembly_line = cpu_isa_.parse_asm(line, \
                    asm_file_pair.first);  
                    if (assembly_line.origin_file() != ASM_INVALID) {
                        asm_prog_.push_back(assembly_line);
                        if (assembly_line.label() != "") {
                            // Update the symbol table if there is a label.
                            symbol_table_.insert({assembly_line.label(), pc_});
                        }
                        // Update the program counter with the previous line 
                        // size.
                        pc_ += assembly_line.size(cpu_isa_);
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
                       asm_file_stack) {
    (void) line;
    (void) next_file;
    (void) asm_file_stack;
}

bool assembler::file_in_list(const \
    std::list<std::pair<std::string, std::ifstream>>& file_list, \
    const std::string& filename) {
    std::list<std::pair<std::string, std::ifstream>>::const_iterator it;
    it = std::find_if(file_list.begin(), file_list.end(), \
    [&filename](const std::pair<std::string, std::ifstream>& element) {
        return element.first == filename;
    });
    return it != file_list.end();
}
