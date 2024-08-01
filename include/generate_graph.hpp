#pragma once

#include <fstream>
#include "suffix_tree.hpp"
using namespace std;


int generate_graph(std::vector<std::string> build_strings, Node *root, string filename);