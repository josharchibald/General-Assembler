// Main file for GenA program
// Revision History:
// 05/06/24 Joshua Archibald Initial Revision.
// 05/07/24 Joshua Archibald Finished command line argument parsing.
// 05/18/24 Joshua Archibald Added assembler object creation.
// 05/20/24 Joshua Archibald Added first pass and updated flags. 

// Used libraries.
#include <cstring>
#include <iostream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include "assembler.hpp"

// Used constants.
const char* FILE_FLAG = "--file";
const char* ISA_FLAG = "--isa";
const char* OUT_FLAG = "--output";
const char* HELP_FLAG = "--help";
const char* VERSION_FLAG = "--version";
const char* LIST_FLAG = "--list";
const char* LOG_FLAG = "--log";
const char* VERBOSE_FLAG = "--verbose";

const char* FILE_FLAG_SHORT = "-f";
const char* ISA_FLAG_SHORT = "-i";
const char* OUT_FLAG_SHORT = "-o";
const char* HELP_FLAG_SHORT = "-h";
const char* VERSION_FLAG_SHORT = "-v";
const char* LIST_FLAG_SHORT = "-l";
const char* LOG_FLAG_SHORT = "-g";
const char* VERBOSE_FLAG_SHORT = "-b";

// Helper functions.

// Displays a usage error message.
void usageError(char *prog) {
	std::cerr << "\nUsage: " << prog << " [options]" << std::endl << "\n" \
	<< "Options:\n" \
	<< "\t-f, --file <main file path>\n" \
	<< "\t\tSpecify the main file path (required).\n" \
	<< "\t-i, --isa <ISA file path>\n" \
	<< "\t\tSpecify the ISA file path (required).\n" \
	<< "\t-o, --output <output folder>\n" \
	<< "\t\tSpecify the output folder path (optional).\n" \
	<< "\t-l, --list\n" \
	<< "\t\tProduce a listing file.\n" \
	<< "\t-g, --log\n" \
	<< "\t\tLog all output to gena.log in the current directory.\n" \
	<< "\t-b, --verbose\n" \
	<< "\t\tOutput all information to the terminal.\n" \
	<< "\t-h, --help\n" \
	<< "\t\tDisplay this help message and exit.\n" \
	<< "\t-v, --version\n" \
	<< "\t\tDisplay the version information and exit.\n" \
	<< "\nNotes:\n" \
	<< "\t- The --output flag is optional. If not specified, the default\n" \
	<< "\t  output folder will be the parent directory of the main file.\n" \
	<< "\t- Both --file and --isa flags must be used with valid paths.\n" \
	<< "\t- Both --log and --verbose flags can be used simultaneously.\n" \
	<< std::endl;
}


// This function takes an argument string, the program string, and modifies a
// string path variable. If the path has already been set, or the argument is
// not a valid path, the usage error will be called. If this isn't the case,
// the path is set to the argument.
void path_flag_handler(std::filesystem::path& path, char *arg, char* prog) {
	// If the path is not empty then the file flag has already been set with
	// a valid path.
	if (!path.empty()) {
		// Usage error is called and the program exits.
		usageError(prog);
	    exit(EXIT_FAILURE);
	}
				
	// If the path has not already been set, see if it's a valid path.
	path = arg;
	// If the path does not exist, display an invalid path error and exit with 
	// a non-zero exit code.
	if (!std::filesystem::exists(path)) {
		std::cerr << "Error: Invalid path: " << path << std::endl;
		exit(EXIT_FAILURE);
	}
}

// This function displays the version and GitHub repo link. This function exits
// with a zero exit code.
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
	bool list, log, verbose;

	// Call the usage error and exit if there are no command line arguments.
	if (argc == 1) {
		// Exits program.
		usageError(argv[0]);
	    exit(EXIT_FAILURE);
	}

	list = false;
	log = false;
	verbose = false;
	// Parse the arguments.
	for (int i = 1; i < argc; i++) {
		// If the main file flag is set, handle it.
		if (((std::strcmp(argv[i], FILE_FLAG) == 0) || 
			 (std::strcmp(argv[i], FILE_FLAG_SHORT) == 0)) && (i != argc - 1)) {
			path_flag_handler(main_file_path, argv[i + 1], argv[0]);
		}
		// If the ISA file flag is set, handle it.
		if (((std::strcmp(argv[i], ISA_FLAG) == 0) || 
			 (std::strcmp(argv[i], ISA_FLAG_SHORT) == 0)) && (i != argc - 1)) {
			path_flag_handler(isa_file_path, argv[i + 1], argv[0]);
		}
		// If the output folder flag is set, handle it.
		if (((std::strcmp(argv[i], OUT_FLAG) == 0) || 
			 (std::strcmp(argv[i], OUT_FLAG_SHORT) == 0)) && (i != argc - 1)) {
			path_flag_handler(output_folder_path, argv[i + 1], argv[0]);
		}
		// If the help flag is set, handle it.
		if ((std::strcmp(argv[i], HELP_FLAG) == 0) || 
			(std::strcmp(argv[i], HELP_FLAG_SHORT) == 0)) {
			// Exit program.
			usageError(argv[0]);
	        exit(EXIT_FAILURE);
		}
		// If the version flag is set, handle it.
		if ((std::strcmp(argv[i], VERSION_FLAG) == 0) || 
			(std::strcmp(argv[i], VERSION_FLAG_SHORT) == 0)) {
			// Exits program.
			version_flag_handler();
		}
		// If the list flag is set, handle it.
		if ((std::strcmp(argv[i], LIST_FLAG) == 0) || 
			(std::strcmp(argv[i], LIST_FLAG_SHORT) == 0)) {
			list = true;
		}
		// If the log flag is set, handle it.
		if ((std::strcmp(argv[i], LOG_FLAG) == 0) || 
			(std::strcmp(argv[i], LOG_FLAG_SHORT) == 0)) {
			log = true;
		}
		// If the verbose flag is set, handle it.
		if ((std::strcmp(argv[i], VERBOSE_FLAG) == 0) || 
			(std::strcmp(argv[i], VERBOSE_FLAG_SHORT) == 0)) {
			verbose = true;
		}
	}

	// If the main file and the ISA file paths are not both filled, call the
	// usage error and exit.
	if (main_file_path.empty() || isa_file_path.empty()) {
		// Exits program.
		usageError(argv[0]);
	    exit(EXIT_FAILURE);
	}
    (void) log; 
    (void) verbose;
    assembler gena(main_file_path, isa_file_path, output_folder_path, verbose,
                   log, list);
    gena.first_pass();
	return 0;
}
