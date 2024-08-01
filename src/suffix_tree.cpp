#include <iostream>
#include <string>
#include <vector>
#include "debug.hpp"
#include "generate_graph.hpp"
#include "suffix_tree.hpp"
using namespace std;


size_t step = 0;


Node* append_node(Node *parent, int string_id, int suffix_offset, int label_offset, int label_length) {
	Node* new_node = new Node(label_offset, label_length);
	new_node->label_string_id = string_id;
	new_node->offsets.push_back({string_id, suffix_offset});
	parent->children.push_back(new_node);
	return new_node;
}


Node* split_node(Node *parent_node, int match_index, int string_id, string original_str, int split_offset) {
	//    (original_offset, original_length)
	// -> (original_offset, parent_length) + (split_offset, child_length)
	// 
	// Ex: split at 4
	//    (3, 7) -> 10
	// -> (3, 1) -> 4   +   (4, 6) -> 10
	//      new            original
	// 
	// Important: original node needs to be end node, to maintain children

	Node *original_node = parent_node->children[match_index];

	int parent_length = split_offset - original_node->label_offset;
	Node* new_node = new Node(original_node->label_offset, parent_length);
	new_node->label_string_id = original_node->label_string_id;

	int child_length = original_node->label_length - parent_length;
	original_node->label_offset = split_offset;
	original_node->label_length = child_length;
	// original_node->suffix_offset = -1;

	new_node->children.push_back(original_node);
	parent_node->children[match_index] = new_node;

	return new_node;
}


int matching_child_index(Node *node, std::vector<std::string> build_strings, string query_str, int query_offset) {
	// If the first character matches, we have found our match.
	// Explanation:   There will never be two children who share the same first character,
	//                as the shared character will have become its own node
	// possible:    a, ba$
	// impossible:  a, aba$

	for (int i = 0; i < node->children.size(); i++) {
		if constexpr(debug) {
			string query_substring = query_str.substr(query_offset, query_str.length());
			string node_label = build_strings[node->children[i]->label_string_id].substr(node->children[i]->label_offset, node->children[i]->label_length);
			cout << node_label << " :|: " << query_substring << '\n';
		}

		if (build_strings[node->children[i]->label_string_id][node->children[i]->label_offset] == query_str[query_offset]) {
			return i;
		}
	}
	return -1;
}


int get_num_shared_chars(Node *node, std::vector<std::string> build_strings, string query_str, int *query_offset) {
	// Cut off label_length from beginning of query string
	// Original:          xyzaba$
	// Node label:        ab     <-- length of 2
	// Query:             aba$   <-- offset 3
	//                           -> Move to the right by 2 (match length)
	// Remaining query:   a$     <-- offset 5
	
	int n = 0;
	// Break loop if either string goes out of bounds
	while (n < node->label_length && *query_offset < query_str.length()) {
		// cout << "Build string: " << build_strings[node->label_string_id] << '\n';
		// cout << "Query string: " << query_str << '\n';
		if (build_strings[node->label_string_id][node->label_offset + n] != query_str[*query_offset]) {
			break;
		}
		n++;
		(*query_offset)++;
	}
	return n;
}


void build_suffix_tree_naive(std::vector<std::string> build_strings, Node *root) {
	// Internal node: children > 0
	// Leaf node: children == 0

	Node *current_node;

	// Loop through all build strings
	for (int i = 0; i < build_strings.size(); i++) {
		const std::string original_str = build_strings[i];

		if constexpr(debug) cout << "\nOriginal terminated string: " << original_str << "\n";

		// Loop through all suffixes
		for (int s = 0; s < original_str.length(); s++) {
			string suffix_label = original_str.substr(s, original_str.length());
			int current_suffix_offset = s;
			if constexpr(debug) cout << "\n---\noffset: " << s << " | suffix: " << suffix_label << '\n';

			// Start with root
			current_node = root;

			// Traverse the tree
			while (current_suffix_offset < original_str.length()) {

				// If children (internal node)
				if (current_node->children.size()) {
					if constexpr(debug) debug_node_info(current_node, build_strings);

					// Check for matching prefix
					if constexpr(debug) cout << "Looking for matching prefix...\n";
					int match_index = matching_child_index(current_node, build_strings, original_str, current_suffix_offset);

					// If child with matching label prefix is found, walk down to matching child node
					if (match_index != -1) {
						if constexpr(debug) cout << "Found match.\n";

						// Do not walk down to child yet, in case we need to split, which involves rewriting parent's reference to child
						Node *matched_child_node = current_node->children[match_index];
						// const auto [matched_child_string_id, matched_child_string_offset] = matched_child_node->string_ids[0];

						// Check for matching characters
						int num_shared_chars = 0;
						// Break loop if either string goes out of bounds
						while (num_shared_chars < matched_child_node->label_length && current_suffix_offset < original_str.length()) {
							if (build_strings[matched_child_node->label_string_id][matched_child_node->label_offset + num_shared_chars] != original_str[current_suffix_offset]) {
								break;
							}
							// Cut off label_length from beginning of suffix
							// Original:          xyzaba$
							// Matching label:    ab     <-- length of 2
							// Suffix:            aba$   <-- offset 3
							//                           -> Move to the right by 2 (matching label length)
							// Remaining suffix:  a$     <-- offset 5
							current_suffix_offset++;
							num_shared_chars++;
						}
						if constexpr(debug) debug_match_child_label(build_strings, i, current_suffix_offset, num_shared_chars);

						// If the whole label matches, start search again from here
						if (num_shared_chars == matched_child_node->label_length) {
							if constexpr(debug) debug_walk_node(matched_child_node, build_strings);
							current_node = matched_child_node;
							if (current_suffix_offset == original_str.length()) {
								current_node->offsets.push_back({i, s});
							}
						}
						// If a partial match, split node
						else {
							if constexpr(debug) debug_split_node(matched_child_node, build_strings, current_suffix_offset, matched_child_node->label_offset + num_shared_chars);
							current_node = split_node(current_node, match_index, i, original_str, matched_child_node->label_offset + num_shared_chars);
							if constexpr(debug) debug_append_node(current_node, build_strings, i, current_suffix_offset);
							append_node(current_node, i, s, current_suffix_offset, original_str.length() - current_suffix_offset);
							if constexpr(step_graph) {
								generate_graph(build_strings, root, "graph/debug/gv/step_" + to_string(step) + ".gv");
								step++;
							}
							break;
						}
					}
					// If no child with matching label is found, append node here
					else {
						if constexpr(debug) cout << "No match found.\n";
						if constexpr(debug) debug_append_node(current_node, build_strings, i, current_suffix_offset);
						append_node(current_node, i, s, current_suffix_offset, original_str.length() - current_suffix_offset);
						if constexpr(step_graph) {
							generate_graph(build_strings, root, "graph/debug/gv/step_" + to_string(step) + ".gv");
							step++;
						}
						break;
					}
				}
				// If no children (leaf), append node here
				else {
					if constexpr(debug) debug_append_node(current_node, build_strings, i, current_suffix_offset);
					append_node(current_node, i, s, current_suffix_offset, original_str.length() - current_suffix_offset);
					if constexpr(step_graph) {
						generate_graph(build_strings, root, "graph/debug/gv/step_" + to_string(step) + ".gv");
						step++;
					}
					break;
				}
			}
		}
	}
}


void find_all_subtree_leaves(Node *node, std::vector<std::string> build_strings, std::vector<std::tuple<int, int>> *matches) {
	if (!node->children.size()) {
		for (const std::tuple offset : node->offsets) {
			matches->push_back(offset);
		}
		return;
	}
	for (int i = 0; i < node->children.size(); i++) {
		find_all_subtree_leaves(node->children[i], build_strings, matches);
	}
}


void query_suffix_tree(Node *root, std::vector<std::string> build_strings, string query_string, std::vector<std::tuple<int, int>> *matches) {
	Node *current_node = root;
	int query_offset = 0;

	while (query_offset < query_string.length()) {
		int match_index = matching_child_index(current_node, build_strings, query_string, query_offset);
		if constexpr(debug) cout << "Match index: " << match_index << '\n';

		// No matching prefix found, fall off the tree
		if (match_index == -1) {
			return;
		}
		else {
			Node *matched_child_node = current_node->children[match_index];
			
			int num_shared_chars = get_num_shared_chars(matched_child_node, build_strings, query_string, &query_offset);
			if constexpr(debug) cout << "Num shared chars: " << num_shared_chars << '\n';
			if constexpr(debug) cout << "Query offset: " << query_offset << '\n';

			// query string exhausted, node label not exhausted   -> full query match, return this node
			// node label exhausted, query string not exhausted   -> full label match, keep searching
			// both exhausted                                     -> exact match, return this node
			// neither exhausted (but broke search)               -> mismatch, fall off the tree

			// Query    -> return match          <---
			// Node     -> keep searching           | --- If query exhausted, return match
			// Both     -> return match          <---
			// Neither  -> return mismatch

			// Partial node match: We don't care when querying, we only cared when
			//                     building the tree to know where to split

			// If the query string has been exhausted (and offset is now out of bounds)
			if (query_offset >= query_string.length()) {
				// Regardless of node label exhaustion, return match
				// Traverse subtree and find all match locations (leaf nodes)
				find_all_subtree_leaves(matched_child_node, build_strings, matches);
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
