// asm_line.hpp 
// Include file for the asm_line class.
// Revision History:
// 05/08/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>
#include <string>

#ifndef ASM_LINE_HPP
#define ASM_LINE_HPP

class asm_line {
	// Publicly usable.
	public:
		// Constructor.
		code_macro(size_t op_code, std::string operand_template, FuncPtr func);
		
		// Destructor.
		~code_macro();

		// Public Methods
		// This function takes in an operand string and returns its 
		// corresponding program data as an int. If an invalid operand is
		// passed in, this function will return NULL and display an error
		// message.
		size_t prog_data(std::string operand);

	// Private usage only.
	private:
		// Private data members.
		// Integer representation of an operation name.
		size_t op_code_;
		// The template for the operands symbols and values.
		std::string operand_template_;
		// Function pointer for translating operands to instructions.
		FuncPtr funcPtr_;
};

#endif // ASM_LINE_HPP
