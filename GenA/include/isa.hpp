// isa.hpp 
// Include file for the isa class.
// Revision History:
// 05/08/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include "code_macro.hpp"
#include <vector>

#ifndef ISA_HPP
#define ISA_HPP
class asm_line;

class isa {
	// Publicly usable.
	public:
		// Constructor.
		// Takes in the isa_file_path as a string to parse the isa file 
		// updating all data. If the file path does not exist NULL will be 
		// returned and an error message will be displayed.
		isa(std::string isa_file_path);
		
		// Destructor.
		~isa();

		// Public Methods
		// This function takes in an operation name and an operand as string
		// objects  and returns its corresponding program data as an int. If an
		// invalid operand or operation name is passed in, this function will
		// return NULL and display an error message.
		size_t translate(std::string op_name, std::string operand);
		
		// This function takes in a line of assembly as a string and returns an
		// asm_line object parsed from that string.If the line of assembly is
		// invalid this function will return NULL and display an error message.
		asm_line parse_asm(std::string);
	
	// Private usage only.
	private:
		// Private data members.
		// Maps operation names to code macros.
		std::unordered_map<std::string, code_macro> code_map_;
		// Holds the order of elements and delimiters in a line of assembly.
		std::vector<std::string> style_;
		// Holds whether or not the processor is Harvard or Princeton arch.
		size_t harv_not_princ_;
		// Holds the word size of the program data, then the data data if they
		// are separate sections.		
		std::vector<size_t> word_sizes_;
		// Holds the memory size, in words, of the program data, then the data
		// data if they are separate sections.
		std::vector<size_t> mem_sizes_;

		// Helper functions.
		// This function takes in a code macro line from the ISA file as a
		// string and parses it updating the code map data member.
		void parse_isa_code_macro(std::string isa_line);

		// This function takes in a string and returns a vector that is the 
		// strings separated by spaces.  
		std::vector<std::string> split_by_spaces(const std::string& str);
};

#endif // ISA_HPP
