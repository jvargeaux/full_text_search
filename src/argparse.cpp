#include <iostream>
#include <memory>
#include <vector>
#include "argparse.hpp"


void ArgParser::print_help() {
	std::cout << "\n\n\033[95;1;4mWelcome to the Suffix Tree!\033[0m\n\n"
	          << "\033[92;1m"
	          << "          |\n"
	          << "         / \\\n"
	          << "        /\\ /\\\n"
	          << "\033[0m\n"
	          << "Positional Arguments:\n"
	          << "\tbuild        Build the suffix tree\n"
	          << "\tquery        Query the suffix tree\n"
	          << "Optional Arguments:\n"
	          << "\t-d, --data   Path to data file\n"
	          << "\t-h, --help   Show this help message\n"
	<< std::endl;
};


ArgParser::ArgumentsList ArgParser::parse_args(int argc, char** argv) {
	ArgParser::ArgumentsList arguments;
	std::vector<std::string> arg_strings;

	if (argc < 2) {
		std::cerr << "\n\033[91;1mSuffix Tree Error: No arguments provided.\033[0m\n";
		ArgParser::print_help();
		exit(EXIT_FAILURE);
	}
	if (argc > 4) {
		std::cerr << "\n\033[91;1mSuffix Tree Error: Too many arguments.\033[0m\n";
		ArgParser::print_help();
		exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i++) {
		arg_strings.push_back(std::string(argv[i]));
		if (arg_strings.back() == "-h" || arg_strings.back() == "--help") {
			ArgParser::print_help();
			exit(EXIT_SUCCESS);
		}
	}

	// Arg 1
	if (arg_strings[0][0] == '-') {
		std::cerr << "\n\033[91;1mSuffix Tree Error: No positional arguments provided.\033[0m\n";
		ArgParser::print_help();
		exit(EXIT_FAILURE);
	}
	if (arg_strings[0] != "build" && arg_strings[0] != "query") {
		std::cerr << "\n\033[91;1mSuffix Tree Error: Invalid positional argument.\033[0m\n";
		ArgParser::print_help();
		exit(EXIT_FAILURE);
	}

	// Arg 2
	if (argc > 2) {
		if (arg_strings[1][0] != '-') {
			std::cerr << "\n\033[91;1mSuffix Tree Error: Too many positional arguments provided.\033[0m\n";
			ArgParser::print_help();
			exit(EXIT_FAILURE);
		}
		if (arg_strings[1] != "-d" && arg_strings[1] != "--data") {
			std::cerr << "\n\033[91;1mSuffix Tree Error: Invalid optional argument.\033[0m\n";
			ArgParser::print_help();
			exit(EXIT_FAILURE);
		}
	}

	// Arg 3
	if (argc == 3) {
		std::cerr << "\n\033[91;1mSuffix Tree Error: No input provided for optional argument.\033[0m\n";
		ArgParser::print_help();
		exit(EXIT_FAILURE);
	}

	arguments.add_argument({ "function" }, arg_strings[0]);
	arguments.add_argument({ "-d", "--data" }, (argc > 3 ? arg_strings[2] : std::string("")));

	return arguments;
};