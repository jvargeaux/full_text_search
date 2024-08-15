#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>
#include "argparse.hpp"
#include "generate_graph.hpp"
#include "suffix_tree.hpp"


int main(int argc, char **argv) {
	ArgParser::ArgumentsList arguments = ArgParser::parse_args(argc, const_cast<char**>(argv));
	arguments.print_all_arguments();

	// std::vector<std::string> build_strings = {"greennerdgreen$", "blargarg$"};
	std::vector<std::string> build_strings = {"green$", "end$"};
	std::string query_string = "en";

	// Build suffix tree
	Node *root = new Node();
	build_suffix_tree_naive(build_strings, root);
	generate_graph(build_strings, root, "graph.gv");

	// Query suffix tree
	std::vector<std::tuple<size_t, size_t>> matches {};
	query_suffix_tree(root, build_strings, query_string, &matches);

	// Print results
	for (const auto &match : matches) {
		size_t string_id = std::get<0>(match);
		size_t string_offset = std::get<1>(match);
		std::cout << "[" << string_id << ", " << string_offset << "]\n";
	}

	// Lambda
	// auto test_fn = []() {}

	// std::cout << arguments.get<std::string>("test") << '\n';
	// std::cout << arguments.get<std::string>("t") << '\n';

	return 0;
}
