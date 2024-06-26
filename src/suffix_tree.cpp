#include <iostream>
#include <string>
#include <vector>
#include "suffix_tree.hpp"
using namespace std;


Node* create_node(Node *parent, char character) {
	Node* node = new Node;
	node->character = character;
	parent->children.push_back(node);
	cout << "Created node. Current nodes size: " << parent->children.size() << '\n';
	for (int i = 0; i < parent->children.size(); i++) {
		cout << parent->children[i]->character;
	}
	cout << '\n';
	return node;
}

void insert_node(Node *root, Node *node) {
	if (root->children.empty()) {
		root->children.push_back(node);
	}
	else {
		for (int i = 0; i < root->children.size(); i++) {
			if (root->children[i]->character == node->character) {
				root->children[i] = node;
				break;
			}
		}
	}
}

int char_index_at_node(Node *node, char c) {
	if (!node->children.empty()) {
		for (int i = 0; i < node->children.size(); i++) {
			if (node->children[i]->character == c) {
				return i;
			}
		}
	}
	return -1;
}

void build_suffix_tree(string str, Node *root) {
	// Loop through all suffixes
	for (int i = 0; i < str.length(); i++) {
		string suffix = str.substr(i, str.length());
		cout << "i: " << i << " | suffix: " << suffix << '\n';

		// For each suffix, walk down the trie and add nodes accordingly
		Node *current_node = root;
		for (int j = 0; j < suffix.length(); j++) {
			int char_index = char_index_at_node(current_node, suffix[j]);
			cout << "char_index of " << suffix[j] << " : " << char_index << '\n';
			// If character doesn't exist
			if (char_index == -1) {
				current_node = create_node(current_node, suffix[j]);
			}
			// If character exists
			else {
				current_node = current_node->children[char_index];
			}
		}
	}
}

int walk_down(string str, Node *root) {
	return 0;
}
