// Main file for GenA program
// Revision History:
// 05/06/24 Joshua Archibald Initial Revision.
// 05/07/24 Joshua Archibald Finished command line argument parsing.

// Used libraries.
#include <cstring>
#include <iostream>
#include <string>
#include <cstdlib> // For system()
#include <filesystem>
#include <fstream>

// Used constants.
const char* FILE_FLAG = "-f";
const char* ISA_FLAG = "-i";
const char* OUT_FLAG = "-o";
const char* HELP_FLAG = "-h";
const char* VERSION_FLAG = "-v";
const char* LIST_FLAG = "-l";


//Helper functions.

// Displays a usage error message and exits the program with non zero exit
// code.
void usageError(char *prog) {
	std::cerr << "\n\tUsage: " << prog << "-f <main file path> " \
	  								  "-i <isa file path> " \
									  "-o <output folder path> " \
									  "-l -h -v" \
								  	  <<  std::endl << "\n\n" << \
	"\tThe help, and version flags, -h and -v, are optional and whichever\n" \
	"\tone comes first in the line will nullify all other flags in the\n" \
    "\tline. The output flag, -o, is also optional. The default output\n" \
	"\tfolder path is the parent directory of the main file If both the\n" \
    "\thelp and the version flags are not used then the main file and ISA\n" \
	"\tfile path flags, -f and -i, must both be used and must both precede\n" \
	"\tvalid paths, including their extensions. A listing file will be\n" \
	"\tproduced with the machine code only if the list flag, -l, is used." \
	<< std::endl << "\n\n";
	exit(EXIT_FAILURE);
}

// This function takes an argument string, the program string,  and takes and
// modifies a string path variable. If the path has already been set, or the
// argument is not a valid path the usage error will be called. If this isn't
// the case, the path is set to the argument.    
void path_flag_handler(std::filesystem::path& path, char *arg, char* prog) {
	// If the path is not empty then the file flag has already been set with
	// a valid path.
	if (!path.empty()) {
		// Usage error is called and the program exits.
		usageError(prog);
	}
				
	// If the path has not already been set see if it's a valid path.
	path = arg;
	// If the path does not exist, display invalid path error and exits with 
	// non zero exit code.
	if (!std::filesystem::exists(path)) {
		std::cerr << "Error: Invalid path: " << path << std::endl;
		exit(EXIT_FAILURE);
	}

}

// This function displays the version and git repo link. This function exits
// with zero exit code.
void version_flag_handler(void) {
	std::cout << "Version: V.1" << std::endl;
	std::cout << "Last updated March: 2024" << std::endl;
	std::cout << "GitHub: https://github.com/josharchibald/General-Assembler" \
			  << std::endl;
	exit(EXIT_SUCCESS);
}


// Main function.
int main(int argc, char* argv[]) {
    // These variables hold the data parsed from the command line arguments. 
	std::filesystem::path main_file_path;
	std::filesystem::path isa_file_path;
	std::filesystem::path output_folder_path;
	bool list;

	// Call the usage error if there are no command line arguments.
	if (argc == 1) {
		// Exits program.
		usageError(argv[0]);
	}
	
	list = false;
	// Parse the arguments.
	for (int i = 1; i < argc; i++) {
		// If the main file flag is set, handle it.
		if ((std::strcmp(argv[i], FILE_FLAG) == 0) && (i != argc - 1)) {
			path_flag_handler(main_file_path, argv[i + 1], argv[0]);
		}

		// If the isa file flag is set, handle it.
		if ((std::strcmp(argv[i], ISA_FLAG) == 0) && (i != argc - 1)) {
			path_flag_handler(isa_file_path, argv[i + 1], argv[0]);
		}

		// If the output folder flag is set, handle it.
		if ((std::strcmp(argv[i], OUT_FLAG) == 0) && (i != argc - 1)) {
			path_flag_handler(output_folder_path, argv[i + 1], argv[0]);
		}
		
		// If the help flag is set, handle it.
		if (std::strcmp(argv[i], HELP_FLAG) == 0) {
			// Exits program.
			usageError(argv[0]);
		}

		// If the version flag is set, handle it.
		if (std::strcmp(argv[i], VERSION_FLAG) == 0) {
			// Exits program.
			version_flag_handler();
		}

		// If the list flag is set, handle it.
		if (std::strcmp(argv[i], LIST_FLAG) == 0) {
			list = true;
		}
	}

	// If the main file and the isa file paths are not both filled, call the
	// usage error. 
	if (!(!main_file_path.empty() && !isa_file_path.empty())) {
		// Exits program.
		usageError(argv[0]);

	}
	

	std::cout << "ISA path: " << isa_file_path << std::endl \
			  << "Main path: " << main_file_path << std::endl \
			  << "List: " << list << std::endl;		
	return 0;
}

