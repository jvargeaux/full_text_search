#include <iostream>
#include <string>
#include "suffix_tree.hpp"
#include "generate_graph.hpp"


int main() {
	string str = "abcdef";
	Node *root = new Node();
	root->character = '#';
	build_suffix_tree(str, root);
	generate_graph(root);
	return 0;
}