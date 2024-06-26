#include <fstream>
#include <iostream>
#include "suffix_tree.hpp"
using namespace std;


void print_tree(Node *node, ofstream *fout) {
	if (node->children.empty()) {
		return;
	}
	for (int i = 0; i < node->children.size(); i++) {
		if (node->character == '#') {
			*fout << '\t' << "root" << " -> " << node->children[i]->character << '\n';
		}
		else {
			*fout << '\t' << node->character << " -> " << node->children[i]->character << '\n';
		}
		print_tree(node->children[i], fout);
	}
}


int generate_graph(Node *root) {
	char filename[] = "graph.gv";
	ofstream fout(filename);

	fout << "digraph {\n";
	print_tree(root, &fout);
	fout << "}" << endl;
	return 0;
}