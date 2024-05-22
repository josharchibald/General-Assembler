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

// Constants.
const size_t ISA_INVALID = std::string::npos;
const std::string PC = "$Val";


class asm_line;

class isa {
	// Publicly usable.
	public:
		// Constructor.
		// Takes in the isa_file_path as a string to parse the isa file 
		// updating all data. If the file path does not exist an invalid ISA
        // will be returned and an error message will be displayed.
		isa(std::string isa_file_path);

		
		// Destructor.
		~isa();

		// Public Methods
		// This function takes in a line of assembly and file path as strings
        // and returns an asm_line object parsed from that string. This function
        // will display an error message if the line of assembly does not match
        // any code macro and the returned asm line will have ASM_INVALID for 
        // each of its data members.
		asm_line parse_asm(std::string line, std::string file_path);
	
		// This function takes in an operation name and an operand as string
		// objects  and returns its corresponding code macro. If an invalid 
        // operand or operation name is passed in, this function will return an
        // invalid code macro and display an error message.
		code_macro code_mac(std::string op_name, std::string operand);

        // Technically these are helper functions but are useful for other 
        // objects.
		// This function takes in a string and returns a vector that is the 
		// strings separated by spaces. 
		std::vector<std::string> split_by_spaces(const std::string& str);
        // This function returns the input but all lowercase and stripped of 
        // white space.
        std::string strip_and_lower(std::string& input);

        // Accessors	
		std::vector<size_t> word_sizes();
		std::vector<size_t> mem_sizes();
		size_t harv_not_princ();
		
	// Private usage only.
	private:
		// Private data members.
		// Holds the word size of the program data, then the data data if they
		// are separate sections.		
		std::vector<size_t> word_sizes_;
		// Holds the memory size, in words, of the program data, then the data
		// data if they are separate sections.
		std::vector<size_t> mem_sizes_;
		// Holds whether or not the processor is Harvard or Princeton arch.
		size_t harv_not_princ_;
		// Holds the order of elements and delimiters in a line of assembly.
		std::vector<std::string> style_;
		// Maps operation names to code macros.
		std::unordered_multimap<std::string, code_macro> code_map_;
        // The file path for the user functions;
        std::string user_function_path_;

		// Helper functions.
        // This file takes in a path to a file and compiles it to a shared 
        // library. If The file can not be compiled an error message is 
        // displayed and the program exits.
        void compile_to_shared_lib(const std::string& source_file);

        // This function takes in the isa file object and the isa file path and 
        // updates the memory data members.
        void parse_isa_mem_data(std::ifstream& isa_file, \
                                std::string isa_file_path);

        // This function takes in a vector of strings and returns true or false
        // the vector represents valid elements and delimiters for the style 
        // data member.   
        bool valid_style(std::vector<std::string> style); 
                                
		// This function takes in a code macro line from the ISA file as a
		// vector of strings and the isa file path as a string and a line 
        // number as an integer and parses the data updating the code map data 
        // member. If any of the data is invalid an error message is displayed.
		void parse_isa_code_macro(std::vector<std::string> isa_line_data, \
                                  std::string isa_file_path, size_t line_num);

        // This function takes in the name of a dynamically linked library and 
        //  function name, both as strings and returns the function pointer to
        // the function.
        code_macro::func_ptr load_function(const std::string& lib_name, \
                            const std::string& func_name);

        // This function takes in a style element and a reference element as 
        // strings, and also takes in an integer cutoff value, and it takes in 
        // and modifies a status variable and a line variable. If the element 
        // matches the type of element the reference is, the delimiter from
        // the original element is used to update the status variable using the
        // line so long as the delimiter is before the cutoff.
        void element_check(std::string element, std::string ref, \
                           std::string& status, std::string& line, \
                           size_t cutoff);
        // This function takes an operand template as a vector of strings and a
        // string operand that gets modified and determines if they match. If 
        // they do a vector of strings is returned with the string arguments, an
        // empty string means the instruction has no arguments. If they don't 
        // then the instructions operand template doesn't match and an empty 
        // vector is returned.
        std::vector<std::string> op_match(std::vector<std::string> op_temp, \
                                          std::string op);
};
#endif // ISA_HPP
