#include <iostream>
#include <string>
#include <vector>
#include "debug.hpp"
#include "generate_graph.hpp"
#include "suffix_tree.hpp"
#include "suffix_tree_ukkonen.hpp"



bool find_match_ukkonen(Node*& active_node, const std::vector<std::string>& build_strings, size_t search_id, size_t& active_child, size_t& active_length, size_t end) {
	if (!(active_node->children.size())) {
		return false;
	}
	
	for (size_t i = 0; i < active_node->children.size(); i++) {
		const auto& child = active_node->children[i];

		// First letter matches
		if (build_strings[child->label_string_id][child->label_offset] == build_strings[search_id][end - active_length]) {
			active_child = i;

			size_t current_label_length = child->label_length != std::string::npos ? child->label_length : (end - child->label_offset);

			size_t num_matching_chars = 1;
			for (size_t a = 1; a < active_length; a++) {
				// Out of bounds
				if ((child->label_offset + a) > current_label_length) {
					break;
				}
				// Match
				if (build_strings[child->label_string_id][child->label_offset + a] == build_strings[search_id][end - active_length + a]) {
					num_matching_chars++;
				}
				else {
					break;
				}
			}

			// Full match
			if (num_matching_chars == current_label_length) {
				active_node = child;
				active_child = std::string::npos;
				active_length = 0;
			}
			// Partial match
			else {
				active_length = num_matching_chars;
			}

			// We've found our match, no need to look further
			return true;
		}
	}
	return false;
}


Node* append_node_ukkonen(Node *parent, size_t string_id, size_t label_offset, size_t suffix_offset) {
	Node* new_node = new Node(string_id, label_offset, std::string::npos);
	new_node->offsets.push_back({string_id, suffix_offset});
	parent->children.push_back(new_node);
	return new_node;
}


Node* split_node_ukkonen(Node *parent_node, size_t child_index, size_t split_offset) {
	Node *original_node = parent_node->children[child_index];

	size_t parent_length = split_offset - original_node->label_offset;
	Node* new_node = new Node(original_node->label_string_id, original_node->label_offset, parent_length);

	size_t child_length = std::string::npos;
	original_node->label_offset = split_offset;
	original_node->label_length = child_length;

	new_node->children.push_back(original_node);
	parent_node->children[child_index] = new_node;

	return new_node;
}


void set_end_position(Node *node, size_t end) {
	// If not root and label_length is unset
	if (node->label_string_id != std::string::npos && node->label_length == std::string::npos) {
		node->label_length = end - node->label_offset;
	}
	for (const auto& child : node->children) {
		set_end_position(child, end);
	}
}


void build_suffix_tree_ukkonen(const std::vector<std::string>& build_strings, Node *root) {

	size_t step = 1;
	for (size_t i = 0; i < build_strings.size(); i++) {
		const std::string& current_build_string = build_strings[i];

		// Temp stack
		Node *active_node = root;
		Node *suffix_link_pointer;
		size_t active_child = std::string::npos;  // active_offset
		size_t active_length = 0;
		size_t remainder = 0;


		for (size_t end = 0; end < current_build_string.length(); end++) {
			std::string current_suffix = current_build_string.substr(0, end + 1);

			// Move on under 2 conditions:
			// - remainder is 0
			// - we found a match

			remainder++;

			while (remainder > 0) {
				// 1. Check if next letter matches where we are
				bool is_match = false;
				// Inside a label
				if (active_child != std::string::npos && active_length) {
					const auto& child = active_node->children[active_child];
					size_t current_label_length = child->label_length != std::string::npos ? child->label_length : (end - child->label_offset);

					if (current_build_string[end] == build_strings[child->label_string_id][child->label_offset + active_length]) {
						active_length++;
						is_match = true;
						// If end of label, set active_node
						if (active_length >= current_label_length) {
							active_node = child;
							active_child = std::string::npos;
							active_length = 0;
						}
					}
				}
				// Search from active node
				else {
					is_match = find_match_ukkonen(active_node, build_strings, i, active_child, active_length, end);
				}


				// 2. If match, move on. Otherwise, append nodes, decrement remainder, go back to step 2. Repeat until remainder is 0.
				if (is_match) {
					// break;
				}
				else {
					// Inside a label
					if (active_child != std::string::npos && active_length) {
						const auto& child = active_node->children[active_child];
						Node *new_internal_node = split_node_ukkonen(active_node, active_child, child->label_offset + active_length);
						if (suffix_link_pointer) {
							new_internal_node->suffix_link = suffix_link_pointer;
						}
						suffix_link_pointer = new_internal_node;

						// Need to get all node label lengths up to this active node

						size_t active_node_label_length = active_node->label_length == std::string::npos ? 0 : active_node->label_length;
						append_node_ukkonen(new_internal_node, i, end, end - active_length - active_node_label_length);

						active_node = root;
						active_child = std::string::npos;
						active_length--;

						if (active_length) {
							find_match_ukkonen(active_node, build_strings, i, active_child, active_length, end);
						}
					}
					// Not inside a label
					else {
						// Need to get all node label lengths up to this active node
						
						size_t active_node_label_length = active_node->label_length == std::string::npos ? 0 : active_node->label_length;
						append_node_ukkonen(active_node, i, end, end - active_length - active_node_label_length);
						active_node = root;
					}
					remainder--;
				}
				

				std::vector<std::string> current_build_strings = {current_suffix};
				if constexpr(debug) generate_graph(current_build_strings, root, "graph/debug/step_" + to_string(step) + ".gv");
				size_t active_node_label_length = active_node->label_length != std::string::npos ? active_node->label_length : (end - active_node->label_offset);
				const std::string& active_node_label = active_node->label_string_id == std::string::npos ? "root" :
					build_strings[active_node->label_string_id].substr(active_node->label_offset, active_node_label_length);
				
				step++;
				// Putting break at end for debugging purposes
				if (is_match) {
					break;
				}
			}
		}

		set_end_position(root, build_strings[i].length());
		if constexpr(debug) generate_graph(build_strings, root, "graph/debug/step_end.gv");
	}
}


size_t matching_child_index_ukkonen(Node *node, const std::vector<std::string>& build_strings, const std::string& query_str, size_t query_offset) {
	for (size_t i = 0; i < node->children.size(); i++) {
		if (build_strings[node->children[i]->label_string_id][node->children[i]->label_offset] == query_str[query_offset]) {
			return i;
		}
	}
	return std::string::npos;
}


size_t get_num_shared_chars_ukkonen(Node *node, const std::vector<std::string>& build_strings, const std::string& query_str, size_t& query_offset) {
	size_t n = 0;
	// Break loop if either string goes out of bounds
	while (n < node->label_length && query_offset < query_str.length()) {
		if (build_strings[node->label_string_id][node->label_offset + n] != query_str[query_offset]) {
			break;
		}
		n++;
		query_offset++;
	}
	return n;
}


void find_all_subtree_leaves_ukkonen(Node *node, const std::vector<std::string>& build_strings, std::vector<std::tuple<size_t, size_t>> *matches) {
	if (!node->children.size()) {
		for (const auto &offset : node->offsets) {
			matches->push_back(offset);
		}
		return;
	}
	for (size_t i = 0; i < node->children.size(); i++) {
		find_all_subtree_leaves_ukkonen(node->children[i], build_strings, matches);
	}
}


void query_suffix_tree_ukkonen(Node *root, const std::vector<std::string>& build_strings, const std::string& query_string, std::vector<std::tuple<size_t, size_t>> *matches) {
	Node *current_node = root;
	size_t query_offset = 0;

	while (query_offset < query_string.length()) {
		size_t match_index = matching_child_index_ukkonen(current_node, build_strings, query_string, query_offset);

		// No matching prefix found, fall off the tree
		if (match_index == std::string::npos) {
			return;
		}
		else {
			Node *matched_child_node = current_node->children[match_index];
			
			size_t num_shared_chars = get_num_shared_chars_ukkonen(matched_child_node, build_strings, query_string, query_offset);

			// If the query string has been exhausted (and offset is now out of bounds)
			if (query_offset >= query_string.length()) {
				// Regardless of node label exhaustion, return match
				// Traverse subtree and find all match locations (leaf nodes)
				find_all_subtree_leaves_ukkonen(matched_child_node, build_strings, matches);
				return;
			}
			// If the query string has not yet been exhausted
			else {
				// If node label has also not been exhausted, fall off the tree (mismatch)
				if (num_shared_chars < matched_child_node->label_length) {
					return;
				}
				// If node label has been exhausted, keep searching
				else {
					current_node = matched_child_node;
				}
			}
		}
	}
}