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
#include <algorithm>
#include <dlfcn.h>

// Constants.
const size_t PRINC_NUM_MEM = 1;
const size_t HARV_NUM_MEM = 2;
const size_t SYTLE_NUM_EL = 4;
const std::string USER_LIB_NAME = "user_lib";
const std::vector<std::string> STYLE_ELEMENTS = {"Label", "OpName", "Operand" \
                                                 "Comment"};
const size_t OP_NAME_IDX = 0;                                                 
const size_t OP_CODE_IDX = 1;
// Reverse indicies 1 indexed.
const size_t FUNC_REV_IDX = 2;
const size_t NUM_BITS_REV_IDX = 1;

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
	if (std::getline(isa_file, isa_line)) {
		compile_to_shared_lib(isa_line);
	}
	// Display error message and exit program if first line is missing and exit.
	else {
		std::cerr << "Error: Missing C++ file path on line 1 of " << \
		             "ISA file: " << isa_file_path << std::endl; 
		exit(EXIT_FAILURE);
	}

    // Parse the next lines for memory information.
    parse_isa_mem_data(isa_file, isa_file_path);

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
		// Otherwise check the validity of the style and update the data.
        if (valid_style(isa_line_data)) {
            style_ = isa_line_data;
        }
        // Display error message and exit if style is not valid.
        else {
            std::cerr << "Error: Invalid line 4 in ISA file: " << \
                          isa_file_path << std::endl; 
            exit(EXIT_FAILURE);
	    }
	}
    // Display error message and exit program if fourth line is missing and 
    // exit.
	else {
		std::cerr << "Error: Missing memory sizes on line 4 of the ISA " << \
		             "file: " << isa_file_path << std::endl; 
		exit(EXIT_FAILURE);
	}

    // Parse code macros for the rest of the file and update code map data.
    while (std::getline(isa_file, isa_line)) {
        isa_line_data = split_by_spaces(isa_line);
        parse_isa_code_macro(isa_line_data, isa_file_path);
    }
}

// Destructor
isa::~isa() {};

// Public functions.
// size_t translate(std::string op_name, std::string operand){}

// Helper functions.
std::vector<std::string> isa::split_by_spaces(const std::string& str) {
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string word;

    while (stream >> word) {
        result.push_back(word);
    }

    return result;
}

void isa::compile_to_shared_lib(const std::string& source_file) {
    std::string command = "g++ -shared -o " + USER_LIB_NAME + " -fPIC " + \
                          source_file;
    if (system(command.c_str()) != 0) {
        std::cerr << "Error: Can not compile the source file: " << source_file \
                     << std::endl;
        exit(EXIT_FAILURE);
    } 
    return;
}

void isa::parse_isa_mem_data(std::ifstream& isa_file, \
                             std::string isa_file_path) {
    std::string isa_line;
	std::vector<std::string> isa_line_data;

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
                    try {
					    word_sizes_.push_back(std::stoul(isa_line_data.at(i)));
                    }
                    // Display error message and exit if string is not a 
                    // positive integer.
                    catch (const std::exception& e) {
                        std::cout << "Error: Invalid entry: " << \
                                     isa_line_data.at(i) << " in line 2 " << \
                                     "of ISA file: " << isa_file_path << \
                                     std::endl;
                        exit(EXIT_FAILURE);
                    }
				}
				break;
			case HARV_NUM_MEM:
				harv_not_princ_ = 1;
				for (size_t i = 0; i < HARV_NUM_MEM; i++) {
					try {
					    word_sizes_.push_back(std::stoul(isa_line_data.at(i)));
                    }
                    // Display error message and exit if string is not a 
                    // positive integer.
                    catch (const std::exception& e) {
                        std::cout << "Error: Invalid entry: " << \
                                     isa_line_data.at(i) << " in line 2 " << \
                                     "of ISA file: " << isa_file_path << \
                                     std::endl;
                    exit(EXIT_FAILURE);
                    }
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
			try {
                mem_sizes_.push_back(std::stoul(isa_line_data.at(i)));
            }
            // Display error message and exit if string is not a 
            // positive integer.
            catch (const std::exception& e) {
                std::cout << "Error: Invalid entry: " << \
                                isa_line_data.at(i) << " in line 3 " << \
                                "of ISA file: " << isa_file_path << \
                                std::endl;
                exit(EXIT_FAILURE);
            }
		}
	}
	// Display error message and exit program if third line is missing.
	else {
		std::cerr << "Error: Missing memory sizes on line 3 of the ISA " << \
		             "file: " << isa_file_path << std::endl; 
		exit(EXIT_FAILURE);
	}
}

bool isa::valid_style(std::vector<std::string> style) {
    std::string element;
    std::vector<std::string> found_elements;
    std::vector<char> found_delimiters;
    bool valid;

    valid = true;
    // Loop through the user specified style.
    for (size_t i; i < style.size(); i++) {
        element = style.at(i);
        // If the element is not in the collection of possible elements, display
        // an error.
        if (std::find(STYLE_ELEMENTS.begin(), STYLE_ELEMENTS.end(), \
            element.substr(0, element.size() - 1)) == STYLE_ELEMENTS.end()) {
            std::cerr << "Error: Invalid element: " << \
            element.substr(0, element.size() - 1) << std::endl; 
            valid = false;
        }
        // If the delimiter is invalid display an error.
        if (std::isalnum(element.back())) {
            std::cerr << "Error: Invalid delimiter: " << element.back() << \
            std::endl; 
            valid = false;
        }
        // If the element has already been found display an error.
        if (std::find(found_elements.begin(), found_elements.end(), \
            element.substr(0, element.size() - 1)) != found_elements.end()) {
            std::cerr << "Error: Duplicate element: " << \
            element.substr(0, element.size() - 1) << std::endl; 
            valid = false;
        }
        // If the delimiter has already been found display an error.
        if (std::find(found_delimiters.begin(), found_delimiters.end(), \
            element.back()) != found_delimiters.end()) {
            std::cerr << "Error: Duplicate delimiter: " << element.back() << \
            std::endl; 
            valid = false;
        }
    }
    return valid;
}

void isa::parse_isa_code_macro(std::vector<std::string> isa_line_data, \
                              std::string isa_file_path) {
    size_t op_code;
    std::vector<std::string> operand_template;
    code_macro::FuncPtr funcPtr;
    size_t num_inst_bits;
    size_t len;
    std::string sym_val;

    len = isa_line_data.size();

    // Convert all data to their necessary types before creating the code macro.
    // If any string is invalid an error message is displayed and the program
    // exits.
    try {
        op_code = std::stoul(isa_line_data.at(OP_CODE_IDX));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: Invalid op code: " << \
                    isa_line_data.at(OP_CODE_IDX) << " at line 4 of ISA " << \
                    "file: " << isa_file_path << std::endl;
        exit(EXIT_FAILURE);
    }

    funcPtr = (code_macro::FuncPtr)load_function(USER_LIB_NAME, \
              isa_line_data.at(len - FUNC_REV_IDX));

    try {
        num_inst_bits = std::stoul(isa_line_data.at(len - NUM_BITS_REV_IDX));
        }
    catch (const std::exception& e) {
        std::cerr << "Error: Invalid number of bits: " << \
                    isa_line_data.at(len - NUM_BITS_REV_IDX) << " at line " << \
                    "4 of ISA file: " << isa_file_path << std::endl;
        exit(EXIT_FAILURE);
    }

    // Special care is taken for the operand template as it may be variable 
    // length.
    size_t i = OP_CODE_IDX + 1;
    sym_val = isa_line_data.at(i);
    while ((sym_val.find("Sym") == 0) || (sym_val == "Val")) {
        operand_template.push_back(sym_val);
        i++;
    }
    // Create the code macro and add it to the code map data member.
    code_macro isa_code_macro(op_code, operand_template, funcPtr, \
                              num_inst_bits);
    code_map_.insert({isa_line_data.at(OP_NAME_IDX), isa_code_macro});
}

void* isa::load_function(const std::string& lib_name, \
                         const std::string& func_name) {
    void* handle = dlopen(lib_name.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error: Cannot open library: " << dlerror() << std::endl;
        return nullptr;
    }

    dlerror(); // reset errors
    void* func = dlsym(handle, func_name.c_str());

    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Error: Cannot load symbol: " << dlsym_error << std::endl;
        dlclose(handle);
        return nullptr;
    }

    return func;
}

