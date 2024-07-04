#include <iostream>
#include <string>
#include "generate_graph.hpp"
#include "suffix_tree.hpp"


int main() {
	string str1 = "greennerdgreen$";
	Node *root = new Node();
	build_suffix_tree_naive(str1, root);
	generate_graph(str1, root, "graph.gv");

	vector<int> matches {};
	query_suffix_tree(root, str1, "n", &matches);

	cout << "\nQuery result: " << '\n';
	for (int m = 0; m < matches.size(); m++) {
		cout << matches[m] << '\n';
	}
	
	return 0;
}
