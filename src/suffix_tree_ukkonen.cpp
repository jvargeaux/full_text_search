#include <iostream>
#include <string>
#include <vector>
#include "debug.hpp"
#include "generate_graph.hpp"
#include "suffix_tree.hpp"
#include "suffix_tree_ukkonen.hpp"



size_t find_match_ukkonen(Node*& active_node, const std::vector<std::string>& build_strings, size_t search_id, size_t& active_child, size_t& active_length, size_t end, size_t remainder, size_t current_suffix_offset) {
	if (!(active_node->children.size())) {
		return 0;
	}

	size_t current_remainder = remainder - current_suffix_offset;

	for (size_t i = 0; i < active_node->children.size(); i++) {
		const auto& child = active_node->children[i];

		// First letter matches
		if (build_strings[child->label_string_id][child->label_offset] == build_strings[search_id][end - current_remainder]) {
			active_child = i;

			size_t current_label_length = child->label_length != std::string::npos ? child->label_length : (end + 1 - child->label_offset);

			size_t num_matching_chars = 1;
			for (size_t a = 1; a < current_remainder; a++) {
				// Out of bounds
				if ((child->label_offset + a) > current_label_length) {
					break;
				}
				// Match
				if (build_strings[child->label_string_id][child->label_offset + a] == build_strings[search_id][end - current_remainder + a]) {
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
			return num_matching_chars;
		}
	}
	return 0;
}


Node* append_node_ukkonen(Node *parent, size_t string_id, size_t label_offset, size_t suffix_offset) {
	Node* new_node = new Node(string_id, label_offset, std::string::npos);
	new_node->offsets.push_back({string_id, suffix_offset});
	parent->children.push_back(new_node);
	// If not root and length is not set
	if (parent->label_string_id != std::string::npos && parent->label_length == std::string::npos) parent->label_length = label_offset - parent->label_offset;
	return new_node;
}


Node* split_node_ukkonen(Node *parent_node, size_t child_index, size_t split_offset) {
	Node *original_node = parent_node->children[child_index];

	size_t parent_length = split_offset - original_node->label_offset;
	Node* new_node = new Node(original_node->label_string_id, original_node->label_offset, parent_length);

	size_t child_length = original_node->label_length != std::string::npos ? (original_node->label_length - split_offset) : std::string::npos;
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

	size_t step = 0;

	for (size_t i = 0; i < build_strings.size(); i++) {
		const std::string& current_build_string = build_strings[i];

		// Temp stack
		Node *active_node = root;
		Node *last_created_node = NULL;
		size_t active_child = std::string::npos;
		size_t active_length = 0;
		size_t remainder = 0;
		size_t current_suffix_offset = 0;


		size_t end = 0;
		remainder++;

		// std::cout << "query string length: " << current_build_string.length() << "\n\n";
		// std::cout << "query string: " << current_build_string << "\n\n";

		while (remainder > 0) {

			// Step Graph
			step++;
			std::string current_suffix = current_build_string.substr(0, end);
			std::vector<std::string> current_build_strings = {current_suffix};
			if constexpr(debug) generate_graph(current_build_strings, root, "graph/debug/step_" + to_string(step) + ".gv");

			// Debug
			size_t active_node_label_length = active_node->label_length != std::string::npos ? active_node->label_length : (end - active_node->label_offset);
			// std::cout << active_node->label_string_id << "\n";
			// std::cout << active_node->label_offset << "\n";
			// std::cout << active_node->label_length << "\n";
			// std::cout << active_node_label_length << "\n";
			// std::cout << build_strings[active_node->label_string_id].length() << "\n";
			// const std::string& active_node_label = (active_node->label_string_id == std::string::npos || active_node->label_offset) ?
			// 	"root" : build_strings[active_node->label_string_id].substr(active_node->label_offset, active_node_label_length);
			// std::cout << "Start Step " << step << "  |  end: " << current_build_string[end] << "(" << end << ")  active_node: " << active_node_label << "  active_child: " << (int)active_child << "  active_length: " << active_length << "  current suffix offset: " << current_suffix_offset << "  remainder: " << remainder << '\n';
			
			// Inside label
			if (active_child != std::string::npos && active_length) {
				const auto& child = active_node->children[active_child];

				// Match
				if (current_build_string[end] == build_strings[child->label_string_id][child->label_offset + active_length]) {
					active_length++;
					current_suffix_offset++;

					// Match entire label
					size_t current_label_length = child->label_length != std::string::npos ? child->label_length : (end + 1 - child->label_offset);
					if (active_length >= current_label_length) {
						active_node = child;
						active_child = std::string::npos;
						active_length = 0;
					}

					// Go next
					if (end < current_build_string.length() - 1) {
						remainder++;
						end++;
					}

					continue;
				}

				// No match
				else {
					const auto& child = active_node->children[active_child];
					Node *new_internal_node = split_node_ukkonen(active_node, active_child, child->label_offset + active_length);
					if (last_created_node && last_created_node->label_length > 1) {
						// Create suffix link from previously created internal node to parent of freshly created internal node (current active node)
						last_created_node->suffix_link = active_node;
					}
					last_created_node = new_internal_node;

					append_node_ukkonen(new_internal_node, i, end, end - current_suffix_offset);

					remainder--;

					// Restart search
					active_node = root;
					active_child = std::string::npos;
					active_length = 0;
					current_suffix_offset = 0;
					continue;
				}
			}

			// At a node
			else {
				size_t num_matching_chars = find_match_ukkonen(active_node, build_strings, i, active_child, active_length, end, remainder - 1, current_suffix_offset);

				// Child
				if (num_matching_chars) {
					current_suffix_offset += num_matching_chars;

					// Check this first -- Match entire remainder
					if (current_suffix_offset >= remainder) {
						// Active child and active length already set from find_match_ukkonen()

						// Go next
						if (end < current_build_string.length() - 1) {
							remainder++;
							end++;
						}
						continue;
					}

					// If entire label was matched, active child was set to null -> Restart search
					if (active_child == std::string::npos) {
						continue;
					}

					// Mismatch at some point
					else {
						// Active child and active length already set from find_match_ukkonen()

						const auto& child = active_node->children[active_child];
						Node *new_internal_node = split_node_ukkonen(active_node, active_child, child->label_offset + active_length);
						if (last_created_node) {
							// Create suffix link from previously created internal node to parent of freshly created internal node (current active node)
							last_created_node->suffix_link = active_node;
						}
						last_created_node = new_internal_node;

						append_node_ukkonen(new_internal_node, i, end, end - current_suffix_offset);

						remainder--;

						// Restart search
						active_node = root;
						active_child = std::string::npos;
						active_length = 0;
						current_suffix_offset = 0;
						continue;
					}
				}

				// No child
				else {
					// active length will always be 0
					append_node_ukkonen(active_node, i, end, end - current_suffix_offset);

					remainder--;

					// Reset to root
					active_node = root;
					active_child = std::string::npos;
					active_length = 0;
					current_suffix_offset = 0;

					// Go next if there's no more remainder, otherwise search from root
					if (remainder < 1 && end < current_build_string.length() - 1) {
						remainder++;
						end++;
					}
					continue;
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


void find_all_subtree_leaves_ukkonen(Node *node, const std::vector<std::string>& build_strings, std::vector<Match>& matches, size_t query_length) {
	if (!node->children.size()) {
		for (const auto &offset : node->offsets) {
			Match new_match = { std::get<0>(offset), std::get<1>(offset), query_length };
			// std::cout << "push: " << std::get<0>(offset) << ", " << std::get<1>(offset) << ", " << query_length << '\n';
			matches.push_back(new_match);
		}
		return;
	}
	for (size_t i = 0; i < node->children.size(); i++) {
		find_all_subtree_leaves_ukkonen(node->children[i], build_strings, matches, query_length);
	}
}


void query_suffix_tree_ukkonen(Node *root, const std::vector<std::string>& build_strings, const std::string& query_string, std::vector<Match>& matches) {
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
				find_all_subtree_leaves_ukkonen(matched_child_node, build_strings, matches, query_string.length());
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


void fuzzy_query_suffix_tree_ukkonen(Node *root, const std::vector<std::string>& build_strings, const std::string& query_string, std::vector<Match>& matches, size_t min_substring) {
	Node *current_node = root;
	string current_query = query_string;
	size_t query_substring_offset = 0;
	size_t matched_query_offset = 0;

	// If node length is 1, there will be no suffix link.

	// Loop through each suffix in query
	while (query_substring_offset < query_string.length()) {
		// Break if current substring is too small
		if (query_string.length() - query_substring_offset < min_substring) {
			break;
		}

		// std::cout << "query substring offset: " << query_substring_offset << "   matched query offset: " << matched_query_offset << '\n';

		size_t match_index = matching_child_index_ukkonen(current_node, build_strings, query_string, matched_query_offset);
		// std::cout << "match index: " << (int)match_index << '\n';

		// No matching prefix found, fall off the tree
		if (match_index == std::string::npos) {
			// We matched something previously
			size_t matched_length = matched_query_offset - query_substring_offset;
			if (matched_query_offset > query_substring_offset && matched_length >= min_substring) {
				find_all_subtree_leaves_ukkonen(current_node, build_strings, matches, matched_length);
			}

			query_substring_offset++;
			matched_query_offset = query_substring_offset;
			// Follow suffix link
			current_node = current_node->suffix_link ? current_node->suffix_link : root;
			current_query = query_string.substr(query_substring_offset);
		}
		else {
			const auto& matched_child_node = current_node->children[match_index];
			size_t num_shared_chars = get_num_shared_chars_ukkonen(matched_child_node, build_strings, query_string, matched_query_offset);

			// End of query, get all positions from current subtree
			if (matched_query_offset >= query_string.length()) {
				find_all_subtree_leaves_ukkonen(matched_child_node, build_strings, matches, query_string.length() - query_substring_offset);

				query_substring_offset++;
				matched_query_offset = query_substring_offset;
				// Follow suffix link
				current_node = matched_child_node->suffix_link ? matched_child_node->suffix_link : root;
				current_query = query_string.substr(query_substring_offset);
			}
			else {
				// Remaining node label = mismatch, end search
				if (num_shared_chars < matched_child_node->label_length) {
					size_t matched_length = matched_query_offset - query_substring_offset;
					if (matched_length >= min_substring) {
						find_all_subtree_leaves_ukkonen(matched_child_node, build_strings, matches, matched_length);
					}

					query_substring_offset++;
					matched_query_offset = query_substring_offset;
					// Follow suffix link
					current_node = matched_child_node->suffix_link ? matched_child_node->suffix_link : root;
					current_query = query_string.substr(query_substring_offset);
				}
				// End of node label, keep searching
				else {
					current_node = matched_child_node;
				}
			}
		}
	}
}