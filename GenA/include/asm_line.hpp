// asm_line.hpp 
// Include file for the asm_line class.
// Revision History:
// 05/15/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>
#include <string>
#include <unordered_map>

#ifndef ASM_LINE_HPP
#define ASM_LINE_HPP
class isa;

class asm_line {
	// Publicly usable.
	public:
		// Constructor.
		// Takes in a line of assembly, as a string, and an isa object and 
		// parses the line updating all data. If the isa object or the line is
		// invalid, NULL will be returned and an error message will be
		// displayed.
		asm_line(std::string line, isa cpu_isa);
		
		// Destructor.
		~asm_line();

		// Public Methods
		// This function takes in an isa object and a symbol table that maps
		// strings to ints, to return the program data corresponding to this 
		// line of assembly as an int. If either argument is invalid, and/or 
		// this asm_line object cannot be assembled using the given arguments,
		// NULL is returned and an error message is displayed.
		size_t assemble(isa cpu_isa, \
			            std::unordered_map<std::string, size_t> symbol_table);
		
		// Accessors
		// All directly from data members.
		std::string origin_file(void);
		std::string text(void);
		std::string label(void);


	// Private usage only.
	private:
		// Private data members.
		// The name of the file the assembly line is from.
		std::string origin_file_name_;
		// The assembly line as text.
		std::string text_;
		// The label in the assembly line.
		std::string label_;  
		// The operation name in the assembly line.
		std::string op_name_;  
		// The operand in the assembly line.
		std::string operand_;  
};

#endif // ASM_LINE_HPP
