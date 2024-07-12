#pragma once

#include <string>
#include <tuple>
#include <vector>


struct Node {
	// std::vector<std::tuple<int, int>> string_ids;
	int label_offset;
	int label_length;
	int suffix_offset;
	std::vector<Node*> children;

	Node(int _label_offset, int _label_length) :
		label_offset(_label_offset), label_length(_label_length), suffix_offset(-1) {}
	Node(): label_offset(0), label_length(0), suffix_offset(-1) {}
};

Node* append_node(Node *node, int label_offset, int label_length);
void build_suffix_tree_naive(std::string str, Node *root);
void query_suffix_tree(Node *root, std::string original_str, std::string query_string, std::vector<int> *matches);

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