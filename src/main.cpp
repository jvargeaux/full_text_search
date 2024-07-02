#include <iostream>
#include <string>
#include "suffix_tree.hpp"
#include "generate_graph.hpp"


int main() {
	string str1 = "greennerdgreen$";
	Node *root = new Node();
	build_suffix_tree_naive(str1, root, true);
	generate_graph(str1, root);
	return 0;
}
