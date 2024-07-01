#include <iostream>
#include <string>
#include "suffix_tree.hpp"
#include "generate_graph.hpp"


int main() {
	string str = "aaba$";
	Node *root = new Node();
	build_suffix_tree_naive(str, root, true);
	generate_graph(str, root);
	return 0;
}
