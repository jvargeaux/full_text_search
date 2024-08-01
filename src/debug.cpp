#include <iostream>
#include <string>
#include "debug.hpp"
#include "suffix_tree.hpp"
using namespace std;


void debug_node_info(Node *current_node, std::vector<std::string> build_strings) {
	string node_label = build_strings[current_node->label_string_id].substr(current_node->label_offset, current_node->label_length);
	if (!node_label.length()) {
		node_label = "<root>";
	}
	cout << "Current node: " << node_label << '\n';
	cout << "Current node children...\n";
	for (int c = 0; c < current_node->children.size(); c++) {
		string child_label = build_strings[current_node->children[c]->label_string_id].substr(current_node->children[c]->label_offset, current_node->children[c]->label_length);
		cout << child_label << '\n';
	}
}


void debug_match_child_label(std::vector<std::string> build_strings, int string_id, int current_suffix_offset, int num_shared_chars) {
	cout << "Num shared chars: " << num_shared_chars << '\n';
	string suffix_label = build_strings[string_id].substr(current_suffix_offset, build_strings[string_id].length());
	cout << "Remaining suffix: " << suffix_label << '\n';
}


void debug_walk_node(Node *current_node, std::vector<std::string> build_strings) {
	cout << "Entire label match.\n";
	string node_label = build_strings[current_node->label_string_id].substr(current_node->label_offset, current_node->label_length);
	cout << "Walk down into node " << node_label << '\n';
}


void debug_split_node(Node *current_node, std::vector<std::string> build_strings, int current_suffix_offset, int split_offset) {
	int parent_length = split_offset - current_node->label_offset;
	int child_length = current_node->label_length - parent_length;
	string original_label = build_strings[current_node->label_string_id].substr(current_node->label_offset, current_node->label_length);
	string parent_label = build_strings[current_node->label_string_id].substr(current_node->label_offset, parent_length);
	string child_label = build_strings[current_node->label_string_id].substr(split_offset, child_length);
	cout << "Partial label match.\n";
	cout << "Split node " << original_label << " into parent " << parent_label << " and child " << child_label << '\n';
	cout << "Walk down into node " << parent_label << '\n';
}


void debug_append_node(Node *current_node, std::vector<std::string> build_strings, int string_id, int current_suffix_offset) {
	string suffix_label = build_strings[string_id].substr(current_suffix_offset, build_strings[string_id].length());
	string node_label = build_strings[current_node->label_string_id].substr(current_node->label_offset, current_node->label_length);
	if (!node_label.length()) {
		node_label = "<root>";
	}
	cout << "Append remaining suffix " << suffix_label << " to node " << node_label << '\n';
}