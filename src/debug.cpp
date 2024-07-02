#include <iostream>
#include <string>
#include "debug.hpp"
#include "suffix_tree.hpp"
using namespace std;


void debug_node_info(Node *current_node, string original_str) {
	string node_label = original_str.substr(current_node->label_offset, current_node->label_length);
	if (!node_label.length()) {
		node_label = "<root>";
	}
	cout << "Current node: " << node_label << '\n';
	cout << "Current node children...\n";
	for (int c = 0; c < current_node->children.size(); c++) {
		string child_label = original_str.substr(current_node->children[c]->label_offset, current_node->children[c]->label_length);
		cout << child_label << '\n';
	}
}


void debug_match_child_label(string original_str, int current_suffix_offset, int num_shared_chars) {
	cout << "Num shared chars: " << num_shared_chars << '\n';
	string suffix_label = original_str.substr(current_suffix_offset, original_str.length());
	cout << "Remaining suffix: " << suffix_label << '\n';
}


void debug_walk_node(Node *current_node, string original_str) {
	cout << "Entire label match.\n";
	string node_label = original_str.substr(current_node->label_offset, current_node->label_length);
	cout << "Walk down into node " << node_label << '\n';
}


void debug_split_node(Node *current_node, string original_str, int current_suffix_offset, int split_offset) {
	int parent_length = split_offset - current_node->label_offset;
	int child_length = current_node->label_length - parent_length;
	string original_label = original_str.substr(current_node->label_offset, current_node->label_length);
	string parent_label = original_str.substr(current_node->label_offset, parent_length);
	string child_label = original_str.substr(split_offset, child_length);
	cout << "Partial label match.\n";
	cout << "Split node " << original_label << " into parent " << parent_label << " and child " << child_label << '\n';
	cout << "Walk down into node " << parent_label << '\n';
}


void debug_append_node(Node *current_node, string original_str, int current_suffix_offset) {
	string suffix_label = original_str.substr(current_suffix_offset, original_str.length());
	string node_label = original_str.substr(current_node->label_offset, current_node->label_length);
	if (!node_label.length()) {
		node_label = "<root>";
	}
	cout << "Append remaining suffix " << suffix_label << " to node " << node_label << '\n';
}