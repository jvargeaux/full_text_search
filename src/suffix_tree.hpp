#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H


#include <vector>
using namespace std;


struct Node {
	char character;
	int index;
	vector<Node*> children;

	Node(): character(0), index(0) {}
};

Node* create_node(char character, int index);
void insert_node(Node *root, Node *node);
void build_suffix_tree(string str, Node *root);
int walk_down(string str, Node *root);


#endif
