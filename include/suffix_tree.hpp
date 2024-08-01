#pragma once

#include <string>
#include <tuple>
#include <vector>


struct Node {
	std::vector<std::tuple<int, int>> offsets;  // [string_id, suffix_offset]
	// string id for label is index 0 of string_info
	int label_string_id;
	int label_offset;
	int label_length;
	std::vector<Node*> children;

	Node(int _label_offset, int _label_length) :
		label_offset(_label_offset), label_length(_label_length) {}
	Node(): label_offset(0), label_length(0) {}
};

Node* append_node(Node *node, int label_offset, int label_length);
void build_suffix_tree_naive(std::vector<std::string> build_strings, Node *root);
void query_suffix_tree(Node *root, std::vector<std::string> build_strings, std::string query_string, std::vector<std::tuple<int, int>> *matches);

// class SuffixTree {
// 	static Node *root;

// 	public:
// 		static void build(std::string build_string) {
// 			if (root) {
// 				delete root;
// 			}
// 			root = new Node();
// 			build_suffix_tree_naive(build_string, root);
// 		};
// 		static void query(std::string query_string) {
// 			printf("%p", root);
// 		};
// };