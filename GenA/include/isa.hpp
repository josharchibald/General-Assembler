// isa.hpp 
// Include file for the isa class.
// Revision History:
// 05/08/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>
#include <string>

#ifndef ISA_HPP
#define ISA_HPP

class isa {
	// Publicly usable.
	public:
		// Constructor.
		// Takes in the isa_file_path as a string to parse the isa file and 
		// update all data.
		isa(std::string isa_file_path);
		
		// Destructor.
		~isa();

		// Public Methods
		// This function takes in an operand string and returns its 
		// corresponding program data as an int. If an invalid operand is
		// passed in, this function will return NULL and display an error
		// message.
		size_t translate(std::string op_name, std::string operand);

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

#endif // ISAL_HPP
