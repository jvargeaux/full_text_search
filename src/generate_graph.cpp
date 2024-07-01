#include <fstream>
#include <iostream>
#include "suffix_tree.hpp"
using namespace std;


void print_tree(string str, Node *node, ofstream *fout) {
	if (node->children.empty()) {
		return;
	}
	string node_label = str.substr(node->label_offset, node->label_length);
	for (int i = 0; i < node->children.size(); i++) {
		string child_label = str.substr(node->children[i]->label_offset, node->children[i]->label_length);
		// No label = root
		if (!node_label.length()) {
			*fout << '\t' << "root" << " -> " << child_label << '\n';
		}
		else {
			*fout << '\t' << node_label << " -> " << child_label << '\n';
		}
		print_tree(str, node->children[i], fout);
	}
}


int generate_graph(string str, Node *root) {
	char filename[] = "graph.gv";
	ofstream fout(filename);

	fout << "digraph {\n";
	print_tree(str, root, &fout);
	fout << "}\n";
	return 0;
}