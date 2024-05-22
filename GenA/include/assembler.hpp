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
#include <sys/ioctl.h>
#include <iostream>
#include <isa.hpp>

#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

class asm_line;

class assembler {
	// Publicly usable.
	public:
		// Constructor.
		// Takes the path to the entry path and the path to the isa file as 
        // strings and initializes all data.
		assembler(std::string entry_path, std::string isa_path, \
                  std::string output_folder_path, bool verbose, bool list);
		
		// Destructor.
		~assembler();

		// Public Methods
        // Performs the first pass on the assembly files. Returns true if
        // successful.
        bool first_pass(void);
        // Performs the second pass the assembly files. Returns true if success.
        bool second_pass(void);


	// Private usage only.
	private:
		// Private data members.
        // The path to the entry point of the program to be assembled.
        std::string entry_path_;
        // Valid assembly file extensions based on entry file.
        std::string valid_extension_;
        // The ISA object for the cpu being assembled.
        isa cpu_isa_; 
        // The path to the output file.
        std::string output_file_path_;
        // Whether to output to terminal or not.
        bool verbose_;        
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
        // the lines pseudo operation. Returns true if successful, and false if
        // not, an error message is also displayed.
        bool pseudo_op_handler(std::string line, bool& next_file, \
        std::vector<std::pair<std::pair<std::string, size_t>, std::ifstream>>& \
        asm_file_stack);
};

#endif // ASSEMBLER_HPP
