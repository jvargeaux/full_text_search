#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H


#include <string>
#include <vector>
using namespace std;


struct Node {
	int label_offset;
	int label_length;
	int string_id;
	int string_offset;
	vector<Node*> children;

	Node(int _label_offset, int _label_length) : label_offset(_label_offset), label_length(_label_length) {}
	Node(): label_offset(0), label_length(0), string_id(-1), string_offset(0) {}
};

Node* append_node(Node *node, int label_offset, int label_length);
void build_suffix_tree_naive(string str, Node *root, bool debug);
int walk_down(string str, Node *root);


#endif
