#pragma once

#include <string>
#include <tuple>
#include <vector>
#include "suffix_tree.hpp"


void build_suffix_tree_ukkonen(const std::vector<std::string>& build_strings, Node *root);
void build_suffix_tree_ukkonen_refactor(const std::vector<std::string>& build_strings, Node *root);
void query_suffix_tree_ukkonen(Node *root, const std::vector<std::string>& build_strings, const std::string& query_string, std::vector<std::tuple<size_t, size_t>> *matches);

