#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include "argparse.hpp"
#include "generate_graph.hpp"
#include "suffix_tree.hpp"


int main(int argc, char **argv) {
	ArgParser::ArgumentsList arguments = ArgParser::parse_args(argc, const_cast<char**>(argv));
	arguments.print_all_arguments();

	string tree_str1 = "greennerdgreen$";
	string query_string = "asfd";

	// Build suffix tree
	// Node *root = new Node();
	// build_suffix_tree_naive(tree_str1, root);
	// generate_graph(tree_str1, root, "graph.gv");

	// Lambda
	// auto test_fn = []() {}

	// std::cout << arguments.get<std::string>("test") << '\n';
	// std::cout << arguments.get<std::string>("t") << '\n';

	return 0;
}
