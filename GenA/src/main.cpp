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
#include <streambuf>

// Used constants.
const char *FILE_FLAG = "--file";
const char *ISA_FLAG = "--isa";
const char *OUT_FLAG = "--output";
const char *HELP_FLAG = "--help";
const char *VERSION_FLAG = "--version";
const char *LIST_FLAG = "--list";
const char *LOG_FLAG = "--log";
const char *VERBOSE_FLAG = "--verbose";
 
const char  *FILE_FLAG_SHORT = "-f";
const char *ISA_FLAG_SHORT = "-i";
const char *OUT_FLAG_SHORT = "-o";
const char *HELP_FLAG_SHORT = "-h";
const char *VERSION_FLAG_SHORT = "-r";
const char *LIST_FLAG_SHORT = "-t";
const char *LOG_FLAG_SHORT = "-l";
const char *VERBOSE_FLAG_SHORT = "-v";
const char *LOG_FILE_NAME = "log_gena";
const char *DEFAULT_OUTPUT_PATH = "output_gena";


// Null stream buffer to discard output
class NullStreamBuf : public std::streambuf {
protected:
    virtual int overflow(int c) override {
        return c; // Discard character
    }
};

// Helper functions.

// Displays a usage error message.
void usageError(char *prog) {
	std::cerr << "\nUsage: " << prog << " [options]" << std::endl << "\n" \
	<< "Options:\n" \
	<< "\t-f, --file <main file path>\n" \
	<< "\t\tSpecify the main file path (required).\n" \
	<< "\t-i, --isa <ISA file path>\n" \
	<< "\t\tSpecify the ISA file path (required).\n" \
	<< "\t-o, --output <output file>\n" \
	<< "\t\tSpecify the output file path (optional).\n" \
	<< "\t-t, --list\n" \
	<< "\t\tProduce a listing file.\n" \
	<< "\t-l, --log\n" \
	<< "\t\tLog all output to gena.log in the current directory.\n" \
	<< "\t-v, --verbose\n" \
	<< "\t\tOutput all information to the terminal.\n" \
	<< "\t-h, --help\n" \
	<< "\t\tDisplay this help message and exit.\n" \
	<< "\t-r, --version\n" \
	<< "\t\tDisplay the version information and exit.\n" \
	<< "\nNotes:\n" \
	<< "\t- The --output flag is optional. If not specified, the default\n" \
	<< "\t  output file will be output_gena.HEX in the working directory .\n" \
	<< "\t- Both --file and --isa flags must be used with valid paths.\n" \
	<< "\t- Both --log and --verbose flags cannot be used simultaneously.\n" \
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
	std::cout << "Last updated May: 2024" << std::endl;
	std::cout << "GitHub: https://github.com/josharchibald/General-Assembler" \
			  << std::endl;
	exit(EXIT_SUCCESS);
}

// Main function.
int main(int argc, char* argv[]) {
    // These variables hold the data parsed from the command line arguments. 
	std::filesystem::path main_file_path;
	std::filesystem::path isa_file_path;
	std::filesystem::path output_file_path;
	bool list, log, verbose, done;

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
			path_flag_handler(output_file_path, argv[i + 1], argv[0]);
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
    std::ofstream log_file;
    std::ostringstream log_name;
    if (log) {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        log_name << LOG_FILE_NAME << std::put_time(&tm, "%M-%S") << ".log";
        log_file.open(log_name.str());
        // Redirect std::cerr and std::clog to the error file.
        if (!log_file.is_open()) {
            std::cerr << "Failed to open or create the log file: " \
            << log_name.str() << std::endl;
            // Exit if the file can't be opened.
            exit(EXIT_FAILURE);
        }
    }
    // Backup original buffers
    std::streambuf* cerr_buf = std::cerr.rdbuf();
    std::streambuf* clog_buf = std::clog.rdbuf();

    if (log && verbose) {
        std::clog << "Warning: Cannot use both log and verbose flags at the " \
        << "same time. Outputting to log file " << log_name.str() << " only." \
        << std::endl;
    }
    if (log) {
        // Redirect std::cerr and std::clog to the file.
        std::cerr.rdbuf(log_file.rdbuf());
        std::clog.rdbuf(log_file.rdbuf());
    } else if (verbose) {
        // Reset std::cerr and std::clog to the console.
        std::cerr.rdbuf(cerr_buf);
        std::clog.rdbuf(clog_buf);
    }
    else {
        NullStreamBuf null_buf;
        // Suppress all output.
        std::cerr.rdbuf(&null_buf);
        std::clog.rdbuf(&null_buf);
    }

    if (output_file_path.empty()) {
        output_file_path = DEFAULT_OUTPUT_PATH;
    }

    // Create and use the assembler object.
    assembler gena(main_file_path, isa_file_path, output_file_path, verbose, \
    list);
    if (gena.first_pass()) {
        done = gena.second_pass();
    }
    else {
        std::cout << "Failed. See log file using -l flag." << std::endl;
    }
    // Reset std::cerr and std::clog to their original buffers before exiting.
    std::cerr.rdbuf(cerr_buf);
    std::clog.rdbuf(clog_buf);

    if (log_file.is_open()) {
        log_file.close();
    }

    if (done) {
        std::cout << main_file_path << " assembled." << std::endl;
    }
    else {
        std::cout << "Failed." << std::endl;
    }
	return 0;
}


