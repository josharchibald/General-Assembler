// assembler.hpp 
// Include file for the assembler class.
// Revision History:
// 05/18/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include "isa.hpp"
#include "asm_line.hpp"

#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

class assembler {
	// Publicly usable.
	public:
		// Constructor.
		// Takes the path to the entry path and the path to the isa file as 
        // strings and initializes all data.
		assembler(std::string entry_path, std::string isa_path, \
                  std::string output_folder_path, bool list);
		
		// Destructor.
		~assembler();

		// Public Methods
        // Performs the first pass on the assembly and include file objects. 
        // This updates the symbol_table, ref_table, asm_prog, and the 
        // data_used data.
        void first_pass (void);


	// Private usage only.
	private:
		// Private data members.
        // The path to the entry point of the program to be assembled.
        std::string entry_path_;
        // The ISA object for the cpu being assembled.
        isa cpu_isa_; 
        // The path to the output folder.
        std::string output_folder_path_;
        // Whether to have a listing output or not.
        bool list_;
        // The program counter static for user library to use.
        size_t pc_;
        // The amount of words of data memory being used.
        size_t data_used_;
        // The symbol table.
		std::unordered_multimap<std::string, size_t> symbol_table_;
        // The collection of assembly lines that are the program itself.
        std::list<asm_line> asm_prog_;
        
        // Helper functions
        // This function takes in a line with a pseudo operation as a string, a 
        // file bool to modify and a file name file pair list to modify and 
        // updates the data members and some args depending on the lines pseudo
        // operation. 
        void pseudo_op_handler(std::string line, bool& next_file, \
             std::list<std::pair<std::string, std::ifstream>>& asm_file_stack);
        // This function takes in the list of file name file pairs and and file
        // name and returns whether the file name is in the list or not.
        bool file_in_list(const \
            std::list<std::pair<std::string, std::ifstream>>& file_list, \
            const std::string& filename);
};

#endif // ASSEMBLER_HPP
