// expression.hpp 
// Include file for the expression class.
// Revision History:
// 05/15/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>
#include <string>

#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

class expression {
	// Publicly usable.
	public:
		// Constructor.
		// Takes in the isa_file_path as a string to parse the isa file 
		// updating all data. If the file path does not exist NULL will be 
		// returned and an error message will be displayed.
		expression(std::string expression);
		
		// Destructor.
		~expression();

		// Public Methods
		// This function returns the expression's int equivalent.
		size_t evaluate(void);
		
	// Private usage only.
	private:
		// Private data members.
		// The expression as a string.
		std::string expression_;

};

#endif // EXPRESSION_HPP
