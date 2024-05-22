// asm_line.hpp 
// Include file for the asm_line class.
// Revision History:
// 05/15/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <assembler.hpp>


#ifndef ASM_LINE_HPP
#define ASM_LINE_HPP

// Constants.
const std::string ASM_INVALID = "";

class isa;

class asm_line {
	// Publicly usable.
	public:
		// Constructor.
        // Takes in and updates all data members.
		asm_line(std::string origin_file_path, std::string text, \
                 std::string label, std::string op_name, std::string operand);
		
		// Destructor.
		~asm_line();

		// Public Methods
		// This function takes in an isa object and a symbol table that maps
		// strings to ints, to return the program data corresponding to this 
		// line of assembly as an int. If either argument is invalid, and/or 
		// this asm_line object cannot be assembled using the given arguments,
		// NULL is returned and an error message is displayed.
		// size_t assemble(isa cpu_isa, 
		// 	            std::unordered_map<std::string, size_t> symbol_table);

        // This function takes in the isa of a cpu and returns the size in bits
        // of this line of assembly.
        size_t size(isa cpu_isa);
        // This function takes in the isa of a cpu and returns the program data
        // as a size_t.
        size_t assemble(isa cpu_isa, \
                        std::unordered_multimap<std::string, size_t> table, \
                        size_t pc);
		
		// Accessors
		// All directly from data members.
		std::string origin_file(void);
		std::string text(void);
		std::string label(void);


	// Private usage only.
	private:
		// Private data members.
		// The path of the file the assembly line is from.
		std::string origin_file_path_;
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
