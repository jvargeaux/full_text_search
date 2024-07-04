#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H


#include <string>
#include <vector>
using namespace std;


struct Node {
	vector<tuple<int, int>> string_ids;
	int label_offset;
	int label_length;
	int suffix_offset;
	vector<Node*> children;

	Node(int _label_offset, int _label_length) :
		label_offset(_label_offset), label_length(_label_length), suffix_offset(-1) {}
	Node(): label_offset(0), label_length(0), suffix_offset(-1) {}
};

Node* append_node(Node *node, int label_offset, int label_length);
void build_suffix_tree_naive(string str, Node *root);
void query_suffix_tree(Node *root, string original_str, string query_string, vector<int> *matches);


#endif
