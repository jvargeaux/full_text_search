#ifndef DEBUG_H
#define DEBUG_H


#include <iostream>
#include <string>
#include "suffix_tree.hpp"
using namespace std;


void debug_node_info(Node *current_node, string original_str);
void debug_match_child_label(string original_str, int current_suffix_offset, int num_shared_chars);
void debug_walk_node(Node *current_node, string original_str);
void debug_split_node(Node *current_node, string original_str, int current_suffix_offset, int split_offset);
void debug_append_node(Node *current_node, string original_str, int current_suffix_offset);


#endif