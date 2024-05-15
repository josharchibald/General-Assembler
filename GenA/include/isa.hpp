// isa.hpp 
// Include file for the isa class.
// Revision History:
// 05/08/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <list>
#include "code_macro.hpp"

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
		std::list<std::string> style_;

};

#endif // ISA_HPP
