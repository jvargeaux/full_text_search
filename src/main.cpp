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
	std::string test_string = "cancann";
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
	std::string query_string = "nn";
	std::vector<std::tuple<size_t, size_t>> matches {};
	query_suffix_tree_ukkonen(root, build_strings, query_string, &matches);

	// Print results
	for (const auto &match : matches) {
		size_t string_id = std::get<0>(match);
		size_t string_offset = std::get<1>(match);
		std::cout << "[" << string_id << ", " << string_offset << "]\n";
	}

	return 0;
}
