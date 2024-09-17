#pragma once

#include <iostream>
#include <string>
#include "suffix_tree.hpp"

constexpr bool debug = false;
constexpr bool step_graph = false;


void debug_node_info(Node *current_node, std::vector<std::string> build_strings);
void debug_match_child_label(std::vector<std::string> build_strings, size_t string_id, size_t current_suffix_offset, size_t num_shared_chars);
void debug_walk_node(Node *current_node, std::vector<std::string> build_strings);
void debug_split_node(Node *current_node, std::vector<std::string> build_strings, size_t split_offset);
void debug_append_node(Node *current_node, std::vector<std::string> build_strings, size_t string_id, size_t current_suffix_offset);