#include <fstream>
#include <iostream>
#include "suffix_tree.hpp"
using namespace std;


size_t id;


void print_node_labels(string str, Node *node, ofstream *fout) {
	if (node->label_length) id++;
	string node_label = str.substr(node->label_offset, node->label_length);
	*fout << '\t' << (node->label_length ? id : 0)
	      << " [label=\"" << (node->label_length ? node_label : "root")
		  << "\n[" << node->label_offset << "," << node->label_length << "]"
		  << " (" << 0 << "," << node->suffix_offset << ")"
		  << "\"]\n";
	if (node->children.empty()) {
		return;
	}
	for (int i = 0; i < node->children.size(); i++) {
		print_node_labels(str, node->children[i], fout);
	}
}


void print_connections(string str, Node *node, ofstream *fout) {
	if (node->children.empty()) {
		return;
	}
	size_t current_id = id;
	for (int i = 0; i < node->children.size(); i++) {
		id++;
		string child_label = str.substr(node->children[i]->label_offset, node->children[i]->label_length);
		*fout << '\t' << (node->label_length ? current_id : 0) << " -> " << id << '\n';
		print_connections(str, node->children[i], fout);
	}
}


int generate_graph(string str, Node *root, string filename = "graph/graph.gv") {
	ofstream fout(filename);

	fout << "digraph {\n";
	id = 0;
	print_node_labels(str, root, &fout);
	id = 0;
	print_connections(str, root, &fout);
	fout << "}\n";
	return 0;
}