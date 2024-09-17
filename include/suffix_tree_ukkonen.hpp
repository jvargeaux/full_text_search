#pragma once

#include <string>
#include <tuple>
#include <vector>
#include "suffix_tree.hpp"


struct Match {
	size_t string_id;
	size_t offset;
	size_t length;

	Match(size_t _string_id, size_t _offset, size_t _length) :
		string_id(_string_id), offset(_offset), length(_length) {}
	Match() : string_id(std::string::npos), offset(std::string::npos), length(0) {}
};


void build_suffix_tree_ukkonen(const std::vector<std::string>& build_strings, Node *root);
void query_suffix_tree_ukkonen(Node *root, const std::vector<std::string>& build_strings, const std::string& query_string, std::vector<std::tuple<size_t, size_t>>& matches);
void fuzzy_query_suffix_tree_ukkonen(Node *root, const std::vector<std::string>& build_strings, const std::string& query_string, std::vector<Match>& matches, size_t min_substring);

