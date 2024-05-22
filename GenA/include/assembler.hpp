// assembler.hpp 
// Include file for the assembler class.
// Revision History:
// 05/18/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include "isa.hpp"
#include "asm_line.hpp"
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

class assembler {
	// Publicly usable.
	public:
		// Constructor.
		// Takes the path to the entry path and the path to the isa file as 
        // strings and initializes all data.
		assembler(std::string entry_path, std::string isa_path, \
                  std::string output_folder_path, bool verbose, bool log,\
                  bool list);
		
		// Destructor.
		~assembler();

		// Public Methods
        // Performs the first pass on the assembly files. 
        void first_pass(void);
        // Performs the second pass the assembly files.
        void second_pass(void);


	// Private usage only.
	private:
		// Private data members.
        // The path to the entry point of the program to be assembled.
        std::string entry_path_;
        // Valid assembly file extensions based on entry file.
        std::string valid_extension_;
        // The ISA object for the cpu being assembled.
        isa cpu_isa_; 
        // The path to the output folder.
        std::string output_folder_path_;
        // Whether to output to terminal or not.
        bool verbose_;        
        // Whether to log output to file or not.
        bool log_;
        // Whether to have a listing output or not.
        bool list_;
        // The program counter static for user library to use.
        size_t pc_;
        // The amount of words of data memory being used.
        size_t data_used_;
        // The symbol table.
		std::unordered_multimap<std::string, size_t> symbol_table_;
        // All file paths used for the assembled program.
        std::unordered_set<std::string> asm_file_paths_;
        // The collection of assembly lines that are the program itself.
        std::list<asm_line> asm_prog_;
        
        // Helper functions
        // This function takes in a line with a pseudo operation as a string, a 
        // file bool to modify and a file name file pair list to modify and 
        // a line number and updates the data members and some args depending on 
        // the lines pseudo operation. 
        void pseudo_op_handler(std::string line, bool& next_file, \
             std::list<std::pair<std::string, std::ifstream>>& asm_file_stack, \
             size_t line_num);
        // Function to get terminal height.
        size_t get_terminal_height();
        // Function to print status at the bottom of the terminal.
        void print_to_user(const std::string& message);
};

#endif // ASSEMBLER_HPP
