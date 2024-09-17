#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>
#include "argparse.hpp"
#include "generate_graph.hpp"
#include "suffix_tree.hpp"
#include "suffix_tree_ukkonen.hpp"


int main(int argc, char **argv) {
	ArgParser::ArgumentsList arguments = ArgParser::parse_args(argc, const_cast<char**>(argv));
	// arguments.print_all_arguments();

	// Load data
	std::filesystem::path data_path {"data/random.csv"};
	const auto data = parse_data(data_path, '\t');
	// for (const auto& _data : data) {
	// 	std::cout << _data.key << ": " << _data.data.size() << '\n';
	// }

	char terminating_char = '$';

	// Check keys match those provided in args

	// Aggregate data to build strings
	std::vector<std::string> build_strings;
	for (const auto& _data : data) {
		for (const auto& line: _data.data) {
			build_strings.push_back(line + terminating_char);
		}
	}

	// Test string
	std::string test_string = "tiringfiringeth";
	// std::string test_string = "fireman hired a firep";
	test_string.push_back(terminating_char);
	build_strings = {test_string};

	// Build suffix tree
	Node *root = new Node();
	std::cout << "Building suffix tree...\n";
	// build_suffix_tree_naive(build_strings, root);
	// generate_graph(build_strings, root, "graph.gv");
	build_suffix_tree_ukkonen(build_strings, root);
	std::cout << "Done.\n";

	// Query suffix tree
	std::string query_string = "thing";
	std::vector<Match> matches {};
	// query_suffix_tree_ukkonen(root, build_strings, query_string, matches);
	fuzzy_query_suffix_tree_ukkonen(root, build_strings, query_string, matches, 2);

	// // Print results
	std::cout << "\nRESULTS\n";
	for (const auto &match : matches) {
		string substring = build_strings[match.string_id].substr(match.offset, match.length);
		std::cout << "\"" << substring << "\": [" << match.string_id << ", " << match.offset << ", " << match.length << "]\n";
	}

	return 0;
}
