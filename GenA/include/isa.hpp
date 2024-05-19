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
// class asm_line;

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
		// asm_line parse_asm(std::string);
	
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
		std::unordered_map<std::string, code_macro> code_map_;

		// Helper functions.
		// This function takes in a string and returns a vector that is the 
		// strings separated by spaces.  
		std::vector<std::string> split_by_spaces(const std::string& str);

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

        // This function takes i the name of a dynamically linked library and 
        //  function name, both as strings and returns the function pointer to
        // the function.
        void* load_function(const std::string& lib_name, \
                            const std::string& func_name);
};

#endif // ISA_HPP
