#include <iostream>
#include <string>
#include <vector>
#include "suffix_tree.hpp"
using namespace std;


Node* append_node(Node *parent, int label_offset, int label_length) {
	Node* new_node = new Node(label_offset, label_length);
	parent->children.push_back(new_node);
	return new_node;
}


Node* split_node(Node *parent_node, int match_index, string original_str, int split_offset) {
	// 		(original_offset, original_length)
	// -> (original_offset, parent_length) + (split_offset, child_length)
	// 
	// Ex:	split at 4
	//		(3, 7) -> 10
	// -> (3, 1) -> 4	 +	(4, 6) -> 10
	//     new    			 original
	// 
	// Important: original node needs to be end node, to maintain children

	Node *original_node = parent_node->children[match_index];

	int parent_length = split_offset - original_node->label_offset;
	Node* new_node = new Node(original_node->label_offset, parent_length);

	int child_length = original_node->label_length - parent_length;
	original_node->label_offset = split_offset;
	original_node->label_length = child_length;

	new_node->children.push_back(original_node);
	parent_node->children[match_index] = new_node;

	return new_node;
}


int matching_prefix_index(Node *node, string original_str, int current_suffix_offset) {
	// If the first character matches, we have found our match.
	// Explanation:		There will never be two children who share the same first character,
	// 								as the shared character will have become its own node
	// possible:		a, ba$
	// impossible:	a, aba$
	for (int i = 0; i < node->children.size(); i++) {
		string suffix_label = original_str.substr(current_suffix_offset, original_str.length());
		string node_label = original_str.substr(node->children[i]->label_offset, node->children[i]->label_length);
		cout << node_label << " :|: " << suffix_label << '\n';

		if (original_str[node->children[i]->label_offset] == original_str[current_suffix_offset]) {
			return i;
		}
	}
	return -1;
}


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


/*
		Internal node: children > 0, string_id == -1
		Leaf node: children == 0, string_id != -1
*/
void build_suffix_tree_naive(string original_str, Node *root, bool debug) {
	cout << "\nOriginal terminated string: " << original_str << "\n";

	Node *current_node;

	// Loop through all suffixes
	for (int s = 0; s < original_str.length(); s++) {
		string suffix_label = original_str.substr(s, original_str.length());
		int current_suffix_offset = s;
		cout << "\n---\noffset: " << s << " | suffix: " << suffix_label << '\n';

		// Start with root
		current_node = root;

		// Traverse the tree
		while (current_suffix_offset < original_str.length()) {

			// If children (internal node)
			if (current_node->children.size()) {
				debug_node_info(current_node, original_str);

				// Check for matching prefix
				cout << "Looking for matching prefix...\n";
				int match_index = matching_prefix_index(current_node, original_str, current_suffix_offset);

				// If child with matching label prefix is found, walk down to matching child node
				if (match_index != -1) {
					cout << "Found match.\n";

					// Do not walk down to child yet, in case we need to split, which involves rewriting parent's reference to child
					Node *matched_child_node = current_node->children[match_index];

					// Check for matching characters
					int num_shared_chars = 0;
					// Break loop if either string goes out of bounds
					while (num_shared_chars < matched_child_node->label_length && current_suffix_offset < original_str.length()) {
						if (original_str[matched_child_node->label_offset + num_shared_chars] != original_str[current_suffix_offset]) {
							break;
						}
						// Cut off label_length from beginning of suffix
						// Original:					xyzaba$
						// Matching label:		ab			<-- length of 2
						// Suffix:						aba$		<-- offset 3
						// 										-> Move to the right by 2 (matching label length)
						// Remaining suffix:	a$			<-- offset 5
						current_suffix_offset++;
						num_shared_chars++;
					}
					debug_match_child_label(original_str, current_suffix_offset, num_shared_chars);

					// If the whole label matches, start search again from here
					if (num_shared_chars == matched_child_node->label_length) {
						debug_walk_node(matched_child_node, original_str);
						current_node = matched_child_node;
					}
					// If a partial match, split node
					else {
						debug_split_node(matched_child_node, original_str, current_suffix_offset, matched_child_node->label_offset + num_shared_chars);
						current_node = split_node(current_node, match_index, original_str, matched_child_node->label_offset + num_shared_chars);
						debug_append_node(current_node, original_str, current_suffix_offset);
						append_node(current_node, current_suffix_offset, original_str.length() - current_suffix_offset);
						break;
					}
				}
				// If no child with matching label is found, append node here
				else {
					cout << "No match found.\n";
					debug_append_node(current_node, original_str, current_suffix_offset);
					append_node(current_node, current_suffix_offset, original_str.length() - current_suffix_offset);
					break;
				}
			}
			// If no children (leaf), append node here
			else {
				debug_append_node(current_node, original_str, current_suffix_offset);
				append_node(current_node, current_suffix_offset, original_str.length() - current_suffix_offset);
				break;
			}
		}
	}
}


int walk_down(string str, Node *root) {
	return 0;
}
