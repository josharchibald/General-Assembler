// isa.cpp
// C++ file for the isa class implementation.
// Revision History:
// 05/18/24 Joshua Archibald Initial revision.
// 05/19/24 Joshua Archibald Implemented parse asm.

// Included libraries.
#include "isa.hpp"
#include "asm_line.hpp"
#include "code_macro.hpp"
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <list>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <dlfcn.h>
#include <cctype>

// Constants.
const size_t PRINC_NUM_MEM = 1;
const size_t HARV_NUM_MEM = 2;
const size_t NUM_STYLE_EL = 3;
const std::string USER_LIB_NAME = \
                  "/home/joshua/General-Assembler/utils/user_lib";
const std::vector<std::string> STYLE_ELEMENTS = {"label", "op_name", "operand"};
const size_t OP_ELEMENT_IDX = 2;
const size_t OP_NAME_ELEMENT_IDX = 1;
const size_t LABEL_ELEMENT_IDX = 0;
const size_t LINE_NUM_START = 4; 
const size_t OP_NAME_IDX = 0;                                                 
const size_t OP_CODE_IDX = 1;
const size_t OP_TEMP_IDX = 2;
// Reverse indicies 1 indexed.
const size_t FUNC_REV_IDX = 2;
const size_t NUM_BITS_REV_IDX = 1;
const std::string COMMENT = ";";
const std::string SYMBOL = "Sym";
const std::string VALUE = "Val";
const std::string PC = "VAL$";

// Constructor.
isa::isa(std::string isa_file_path) {
	std::string isa_line;
	std::vector<std::string> isa_line_data;
    size_t line_num;

    line_num = 0;

	// Create the isa file object. If the file can't be opened display
	// an error message and exit the program. 
	std::ifstream isa_file(isa_file_path);
	if (!isa_file) {
		std::cerr << "Error: Unable to open file: " << isa_file_path \
				  << std::endl;
		exit(EXIT_FAILURE);
	}
    
    // Avoid comments and empty lines.
    isa_line = "";
    while(std::getline(isa_file, isa_line)) {
        if ((isa_line.find(COMMENT) != 0) || isa_line.empty()) {
            line_num++;
            break;
        }
        isa_line = "";
    }
	// Parse the first line of the ISA file and set up the DLL with specified
	// C++ file path.
	if (isa_line != "") {
		compile_to_shared_lib(isa_line);
	}
	// Display error message and exit program if first line is missing and exit.
	else {
		std::cerr << "Error: Missing C++ file path in " << "ISA file: " << \
        isa_file_path << std::endl; 
		exit(EXIT_FAILURE);
	}

    // Parse the next lines for memory information.
    parse_isa_mem_data(isa_file, isa_file_path);

    // Avoid comments.
    isa_line = "";
    while(std::getline(isa_file, isa_line)) {
        if ((isa_line.find(COMMENT) != 0) || isa_line.empty()) {
            line_num++;
            break;
        }
        isa_line = "";
    }
	// Parse the fourth line of the ISA file and update the style data member.
	if (isa_line != "") {
		// Split by spaces.
		isa_line_data = split_by_spaces(isa_line);
		// If the fourth line does not contain all needed elements, display an
		// error message and exit.
		if (isa_line_data.size() != NUM_STYLE_EL) {
			std::cerr << "Error: Missing line elements on syntax line the " << \
		    	         "ISA file:" << isa_file_path << std::endl; 
			exit(EXIT_FAILURE);
		}
		// Otherwise check the validity of the style and update the data.
        if (valid_style(isa_line_data)) {
            for (size_t i = 0; i < isa_line_data.size(); i++) {
                // If the element delimiter is alphanumeric assume the user meant the 
                // the white space itself as the delimiter.
                if (std::isalnum(isa_line_data.at(i).back())) {
                    isa_line_data.at(i) += ' ';
                }
            }
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
		std::cerr << "Error: Missing memory sizes in the ISA " << "file: " << \
        isa_file_path << std::endl; 
		exit(EXIT_FAILURE);
	}

    // Parse code macros for the rest of the file and update code map data.
    line_num = line_num + LINE_NUM_START;
    while (std::getline(isa_file, isa_line)) {
        // Avoid comments.
        if ((isa_line.find(COMMENT) == 0) || isa_line.empty()) {
            line_num++;
            continue;
        }
        isa_line_data = split_by_spaces(isa_line);
        parse_isa_code_macro(isa_line_data, isa_file_path, line_num);
        line_num++;
    }
    isa_file.close();
}

// Destructor
isa::~isa() {};

// Public functions.
asm_line isa::parse_asm(std::string line, std::string file_path) {
    std::string label;
    std::string op_name;
    std::string operand;
    std::string text;
    std::string asm_file_path;
    std::string element;
    size_t cutoff;

    text = line;
    asm_file_path = file_path;
    // Go through each line elements.
    for (size_t i = 0; i < NUM_STYLE_EL; i++) {
        // Find where the next closest present element is. This cuts off where 
        // to look for the current element.
        cutoff = std::string::npos;
        for (size_t j = i + 1; j < NUM_STYLE_EL; j++) {
            element = style_.at(j);
            if (line.find(element.back()) < cutoff) {
                cutoff = line.find(element.back());
            }
        }

        // Check for which element to update.
        element = style_.at(i);
        element_check(element, STYLE_ELEMENTS.at(LABEL_ELEMENT_IDX), label, \
                      line, cutoff);
        element_check(element, STYLE_ELEMENTS.at(OP_NAME_ELEMENT_IDX), \
                      op_name, line, cutoff);
        element_check(element, STYLE_ELEMENTS.at(OP_ELEMENT_IDX), operand, \
                      line, cutoff);
    }

    label = strip_and_lower(label);
    op_name = strip_and_lower(op_name);
    operand = strip_and_lower(operand);

    // If the asm line has no matching code macro display an error message and
    // invalidate the asm_line.
    if ((operand != "") && (code_mac(op_name, operand).num_inst_bits() == \
                            ISA_INVALID)) {
        std::cerr << "Error: No code macro found for " << op_name << " " << \
        operand << std::endl;
        return asm_line(ASM_INVALID, ASM_INVALID, ASM_INVALID, ASM_INVALID, \
                        ASM_INVALID);
    }
    return asm_line(asm_file_path, text, label, op_name, operand);
}

code_macro isa::code_mac(std::string op_name, std::string operand) {
    std::vector<std::string> op_temp;
    auto macros = code_map_.equal_range(op_name);
    for (auto macro = macros.first; macro != macros.second; ++macro) {
        op_temp = macro->second.operand_template();
        if (op_match(op_temp, operand)) {
            return macro->second;
        }
    }
    // Return invalid code macro and display error message if none found.
    return code_macro(ISA_INVALID, std::vector<std::string>(), NULL, \
                      ISA_INVALID);
}

// Helper functions.
bool isa::op_match(std::vector<std::string> op_temp, std::string& op) {
    bool prev_val = false;
    // Go through each sting in the template.
    for (std::string sym : op_temp) {
        if (sym.find(SYMBOL) == 0) {
            sym = sym.substr(SYMBOL.length());
            // If for any SYMBOL symbol in the template the string designated to
            // it is not in the remaining operand, return false.
            if (!op.find(sym)) {
                return false;
            }
            // If the string is found, and the previous symbol was a value, the
            // operand can be cut off to the string symbol regardless of if it
            // is at the front of the remaining operand.
            else if (prev_val) {
                op = op.substr(op.find(sym) + sym.length());
            }
            // If the previous symbol was not a value it must be checked that 
            // the string SYMBOL is at the start of the line before cutting it
            else if (op.find(sym) == 0) {
                op = op.substr(op.find(sym) + sym.length());
            }
            // If its not at the start and the symbol before was not a value 
            // then the operand does not match.
            else {
                return false;
            }
            prev_val = false;
        }
        else if (sym == VALUE) {
            prev_val = true;
        }
    }
    // Since the op has been cut off of all symbols and values between them, the
    // remaining operand must either be an empty string, or the last symbol must
    // be a value for the operand to match the template.
    return (op == "") || (op_temp.back() == VALUE);
}

void isa::element_check(std::string element, std::string ref, std::string& \
                        status, std::string& line, size_t cutoff) {
    // Delimiter index.
    size_t i;

    // Any element may be delimited by the end of the line.
    i =  std::min(line.find(element.back()), line.size());
    // The status string is updated with the string leading to its elements 
    // delimiter if its equal to the reference element and the delimiter is not
    // past the cut off.
    if ((element.substr(0, element.size() - 1) == ref) && (i < cutoff)) {
        status = line.substr(0, i);
        line = line.substr(std::min(i + 1, line.size()));
        size_t start = line.find_first_not_of(' ');
        if (start != std::string::npos) {
            line = line.substr(start);
        }
    }
}

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

    // Avoid comments.
    isa_line = "";
    while(std::getline(isa_file, isa_line)) {
        if ((isa_line.find(COMMENT) != 0) || isa_line.empty()) {
            break;
        }
        isa_line = "";
    }
    // Parse the second line of the ISA file and update the word size data
	// member.
	if (isa_line != "") {
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
                                     isa_line_data.at(i) << " on word line " \
                                     << "of ISA file: " << isa_file_path << \
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
                                     isa_line_data.at(i) << " on word line " \
                                     << "of ISA file: " << isa_file_path << \
                                     std::endl;
                    exit(EXIT_FAILURE);
                    }
				}
				break;
			// Display error message and exit if the wrong number of strings 
			// are on the second line.
			default:
				std::cerr << "Error: Wrong number of word sizes on word " << \
				             "line of the ISA file: " << isa_file_path << \
							 std::endl; 
				exit(EXIT_FAILURE);
		}
	}
	// Display error message and exit program if second line is missing.
	else {
		std::cerr << "Error: Missing word sizes on word line of the ISA " << \
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
				         "word and size lines of the ISA file " << \
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
                                isa_line_data.at(i) << " in memory size " << \
                                "line of ISA file: " << isa_file_path << \
                                std::endl;
                exit(EXIT_FAILURE);
            }
		}
	}
	// Display error message and exit program if third line is missing.
	else {
		std::cerr << "Error: Missing memory sizes memory size line 3 of " << \
		             "the ISA file: " << isa_file_path << std::endl; 
		exit(EXIT_FAILURE);
	}
    return;
}

bool isa::valid_style(std::vector<std::string> style) {
    std::string element;
    std::vector<std::string> found_elements;
    std::vector<char> found_delimiters;
    bool valid;

    valid = true;
    // Loop through the user specified style.
    for (size_t i = 0; i < style.size(); i++) {
        element = style.at(i);
        // If the element delimiter is alphanumeric assume the user meant the 
        // the white space itself as the delimiter.
        if (std::isalnum(element.back())) {
            element += ' ';
        }

        // If the element is not in the collection of possible elements, display
        // an error.
        if (std::find(STYLE_ELEMENTS.begin(), STYLE_ELEMENTS.end(), \
            element.substr(0, element.size() - 1)) == STYLE_ELEMENTS.end()) {
            std::cerr << "Error: Invalid element: " << \
            element.substr(0, element.size() - 1) << std::endl; 
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
                              std::string isa_file_path, size_t line_num) {
    size_t op_code;
    std::vector<std::string> operand_template;
    code_macro::func_ptr func;
    size_t num_inst_bits;
    size_t len;
    std::string sym_val;
    bool make;

    make = true;
    len = isa_line_data.size();

    // Convert all data to their necessary types before creating the code macro.
    // If any string is invalid an error message is displayed and the code macro
    // is not made.
    op_code = 0;
    try {
        op_code = std::stoul(isa_line_data.at(OP_CODE_IDX));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: Invalid op code: " << \
                    isa_line_data.at(OP_CODE_IDX) << " at line "<< line_num << \
                    " of ISA " << "file: " << isa_file_path << std::endl;
        make = false;
    }

    for (size_t i = OP_TEMP_IDX; i < len - FUNC_REV_IDX; i++) {
        sym_val = isa_line_data.at(i);
        if ((sym_val.find(SYMBOL) != 0) && (sym_val != VALUE) && (sym_val != \
             PC)) {
            std::cerr << "Error: Invalid symbol: " << sym_val << "at line " << \
                         line_num << " of ISA file: " << isa_file_path << \
                         std::endl;
            make = false;
        }
        else {
            operand_template.push_back(sym_val);
        }
    }

    func = (code_macro::func_ptr)load_function(USER_LIB_NAME, \
              isa_line_data.at(len - FUNC_REV_IDX));
    
    if (func == NULL) {
        make = false;
    }

    num_inst_bits = 0;
    try {
        num_inst_bits = std::stoul(isa_line_data.at(len - NUM_BITS_REV_IDX));
        }
    catch (const std::exception& e) {
        std::cerr << "Error: Invalid number of bits: " << \
                    isa_line_data.at(len - NUM_BITS_REV_IDX) << " at line " << \
                    line_num << " of ISA file: " << isa_file_path << std::endl;
        make = false;
    }


    // Create the code macro and add it to the code map data member if all data
    // is good.
    if (make) {
        code_macro isa_code_macro(op_code, operand_template, func, \
                                num_inst_bits);
        code_map_.insert({strip_and_lower(isa_line_data.at(OP_NAME_IDX)), \
                                          isa_code_macro});
    }
    return;
}

void* isa::load_function(const std::string& lib_name, \
                         const std::string& func_name) {
    void* handle = dlopen(lib_name.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error: Cannot open library: " << dlerror() << std::endl;
        return NULL;
    }

    dlerror(); // reset errors
    void* func = dlsym(handle, func_name.c_str());

    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Error: Cannot load symbol: " << dlsym_error << std::endl;
        dlclose(handle);
        return NULL;
    }

    return func;
}

std::string isa::strip_and_lower(std::string& input) {
    std::string result;
    // Copy only non-whitespace characters to result
    std::copy_if(input.begin(), input.end(), std::back_inserter(result), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
    // Convert result to lowercase
    std::transform(result.begin(), result.end(), result.begin(), [](char c) {
        return std::tolower(static_cast<unsigned char>(c));
    });
    return result;
}

