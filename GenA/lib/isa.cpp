// isa.cpp
// C++ file for isa class implementation.
// Revision History:
// 05/18/24 Joshua Archibald Initial revision.

// Included libraries.
#include "isa.hpp"
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <list>
#include <iostream>
#include <fstream>
#include "code_macro.hpp"
#include <vector>
#include <sstream>
#include <cstdlib>

// Constants.
const size_t PRINC_NUM_MEM = 1;
const size_t HARV_NUM_MEM = 2;
const size_t SYTLE_NUM_EL = 4;

// Constructor.
isa::isa(std::string isa_file_path) {
	std::string isa_line;
	std::vector<std::string> isa_line_data;

	// Create the isa file object. If the file can't be opened display
	// an error message and exit the program. 
	std::ifstream isa_file(isa_file_path);
	if (!isa_file) {
		std::cerr << "Error: Unable to open file: " << isa_file_path \
				  << std::endl;
		exit(EXIT_FAILURE);
	}
	
	// Parse the first line of the ISA file and set up the DLL with specified
	// C++ file path.
	// TODO: Set up DLL.
	if (std::getline(isa_file, isa_line)) {
		std::cout << "DLL" << std::endl;
	}
	// Display error message and exit program if first line is missing.
	else {
		std::cerr << "Error: Missing C++ file path on line 1 of " << \
		             "ISA file: " << isa_file_path << std::endl; 
		exit(EXIT_FAILURE);
	}

	// Parse the second line of the ISA file and update the word size data
	// member.
	if (std::getline(isa_file, isa_line)) {
		// Split the line by spaces.
		isa_line_data = split_by_spaces(isa_line);
		// Set the memory architecture and save the word sizes.
		switch (isa_line_data.size()) {
			case PRINC_NUM_MEM:
				harv_not_princ_ = 0;
				for (size_t i = 0; i < PRINC_NUM_MEM; i++) {
					word_sizes_.push_back(std::stoi(isa_line_data.at(i)));
				}
				break;
			case HARV_NUM_MEM:
				harv_not_princ_ = 1;
				for (size_t i = 0; i < HARV_NUM_MEM; i++) {
					word_sizes_.push_back(std::stoi(isa_line_data.at(i)));
				}
				break;
			// Display error message and exit if the wrong number of strings 
			// are on the second line.
			default:
				std::cerr << "Error: Wrong number of word sizes on line " << \
				             "2 of the ISA file: " << isa_file_path << \
							 std::endl; 
				exit(EXIT_FAILURE);
		}
	}
	// Display error message and exit program if second line is missing.
	else {
		std::cerr << "Error: Missing word sizes on line 2 of the ISA " << \
			         "file: " << isa_file_path << std::endl; 
		exit(EXIT_FAILURE);
	}

	// Parse the third line of the ISA file and update the memory size data
	// member.
	if (std::getline(isa_file, isa_line)) {
		// Split by spaces.
		isa_line_data = split_by_spaces(isa_line);
		// If the number of strings on this line is not consistent with the 
		// previous line, display error message and exit program.
		if ((harv_not_princ_ + 1) != isa_line_data.size()) {
			std::cerr << "Error: Memory space number inconsistent " << \
				         "between lines 2 and 3 of the ISA file " << \
		             	 isa_file_path << std::endl; 
			exit(EXIT_FAILURE);
		}
		// Otherwise save all the memory size data.
		for (size_t i = 0; i < isa_line_data.size(); i++) {
			mem_sizes_.push_back(std::stoi(isa_line_data.at(i)));
		}
	}
	// Display error message and exit program if third line is missing.
	else {
		std::cerr << "Error: Missing memory sizes on line 3 of the ISA " << \
		             "file: " << isa_file_path << std::endl; 
		exit(EXIT_FAILURE);
	}

	// Parse the fourth line of the ISA file and update the style data member.
	if (std::getline(isa_file, isa_line)) {
		// Split by spaces.
		isa_line_data = split_by_spaces(isa_line);
		// If the fourth line does not contain all needed elements, display an
		// error message and exit.
		if (isa_line_data.size() != SYTLE_NUM_EL) {
			std::cerr << "Error: Missing line elements on line 4 of the " << \
		    	         "ISA file:" << isa_file_path << std::endl; 
			exit(EXIT_FAILURE);
		}
		// Otherwise 
	}
}
	
size_t translate(std::string op_name, std::string operand);
		
void parse_isa_line(std::string isa_line) {

}

std::vector<std::string> split_by_spaces(const std::string& str) {
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string word;

    while (stream >> word) {
        result.push_back(word);
    }

    return result;
}
