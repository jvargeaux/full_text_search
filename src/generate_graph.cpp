#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include "suffix_tree.hpp"
using namespace std;


size_t id;


void print_node_labels(std::vector<std::string> build_strings, Node *node, ofstream *fout) {

	if (node->label_length) id++;
	string node_label = build_strings[node->label_string_id].substr(node->label_offset, node->label_length);

	*fout << '\t' << (node->label_length ? id : 0)
	      << " [label=\"" << (node->label_length ? node_label : "root")
		  << "\n[" << node->label_string_id << "," << node->label_offset << "," << node->label_length << "]";
	for (size_t i = 0; i < node->offsets.size(); i++) {
		size_t string_id = std::get<0>(node->offsets[i]);
		size_t string_offset = std::get<1>(node->offsets[i]);
		*fout << " (" << string_id << "," << string_offset << ")";
	}
	*fout << "\"]\n";

	if (node->children.empty()) {
		return;
	}
	for (size_t i = 0; i < node->children.size(); i++) {
		print_node_labels(build_strings, node->children[i], fout);
	}
}


void print_connections(std::vector<std::string> build_strings, Node *node, ofstream *fout) {
	if (node->children.empty()) {
		return;
	}
	size_t current_id = id;
	for (size_t i = 0; i < node->children.size(); i++) {
		id++;
		string child_label = build_strings[node->children[i]->label_string_id].substr(node->children[i]->label_offset, node->children[i]->label_length);
		*fout << '\t' << (node->label_length ? current_id : 0) << " -> " << id << '\n';
		print_connections(build_strings, node->children[i], fout);
	}
}


void generate_graph(std::vector<std::string> build_strings, Node *root, string filename = "graph/graph.gv") {
	ofstream fout(filename);

	fout << "digraph {\n";
	id = 0;
	print_node_labels(build_strings, root, &fout);
	id = 0;
	print_connections(build_strings, root, &fout);
	fout << "}\n";
}