// operator.hpp 
// Include file for the operator class.
// Revision History:
// 05/15/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>
#include <string>

#ifndef OPERATOR_HPP
#define OPERATOR_HPP

class expr_operator {
	// Publicly usable.
	public:
		using FuncPtr = void (*)();
		// Constructor.
		// Takes in and updates all data. If invalid arguments are passed, NULL
		// is returned and an error message is displayed.
		expr_operator(size_t priority, std::string symbol, FuncPtr eval_func);
		
		// Destructor.
		~expr_operator();

		// Public Methods
		// This function takes in two integer argument and returns the result
		// of this operator's operation.
		size_t evaluate(size_t arg1, size_t arg2);
		
		// Accessors
		// All directly from data members.
		size_t priority(void);
		std::string symbol(void);


	// Private usage only.
	private:
		// Private data members.
		// The operator's priority.
		size_t priority_;  
		// The operators string representation.
		std::string symbol_;  
		// The pointer to the function that evaluates this operator.
		FuncPtr	eval_func_;  
};

#endif // OPERATOR_HPP
