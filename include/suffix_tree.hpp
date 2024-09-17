#pragma once

#include <filesystem>
#include <string>
#include <tuple>
#include <vector>


struct ParsedData {
	std::string key;
	std::vector<std::string> data;
};


struct Node {
	size_t label_string_id;
	size_t label_offset;
	size_t label_length;
	std::vector<std::tuple<size_t, size_t>> offsets;  // [string_id, suffix_offset]
	std::vector<Node*> children;
	Node* suffix_link;

	Node(size_t _label_string_id, size_t _label_offset, size_t _label_length) :
		label_string_id(_label_string_id), label_offset(_label_offset), label_length(_label_length) {}
	Node(): label_string_id(std::string::npos), label_offset(std::string::npos), label_length(std::string::npos) {}
};

std::vector<ParsedData> parse_data(const std::filesystem::path& data_path, char separator_char);
Node* append_node(Node *parent, size_t string_id, size_t suffix_offset, size_t label_offset, size_t label_length);
void build_suffix_tree_naive(const std::vector<std::string>& build_strings, Node *root);
void query_suffix_tree(Node *root, const std::vector<std::string>& build_strings, const std::string& query_string, std::vector<std::tuple<size_t, size_t>> *matches);
