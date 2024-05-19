// assembler.hpp 
// Include file for the assembler class.
// Revision History:
// 05/18/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>
#include <string>
#include <vector>
#include "isa.hpp"

#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

class assembler {
	// Publicly usable.
	public:
		// Constructor.
		// Takes the path to the entry path and the path to the isa file as 
        // strings and initializes all data.
		assembler(std::string entry_path, std::string isa_path);
		
		// Destructor.
		~assembler();

		// Public Methods


	// Private usage only.
	private:
		// Private data members.
        // The path to the entry point of the program to be assembled.
        std::string entry_path_;
        // The ISA object for the cpu being assembled.
        isa cpu_isa_; 
};

#endif // CODE_MACRO_HPP
