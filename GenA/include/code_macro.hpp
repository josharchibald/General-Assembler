// code_macro.hpp 
// Include file for the code_macro class.
// Revision History:
// 05/07/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>
#include <string>
#include <vector>

#ifndef CODE_MACRO_HPP
#define CODE_MACRO_HPP

class code_macro {
	// Publicly usable.
	public:
		using func_ptr = void (*)();
		// Constructor.
		// Takes in, and updates all data.
		code_macro(size_t op_code, std::vector<std::string> operand_template, \
                   func_ptr func, size_t num_inst_bits);
		
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
		std::vector<std::string> operand_template_;
		// Function pointer for translating operands to instructions.
		func_ptr func_;
        // Number of bits in the instruction.
        size_t num_inst_bits_;
};

#endif // CODE_MACRO_HPP
