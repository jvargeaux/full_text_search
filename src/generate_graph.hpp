#ifndef GENERATE_GRAPH_H
#define GENERATE_GRAPH_H


#include <fstream>
#include "suffix_tree.hpp"
using namespace std;


void print_tree(Node *root, ofstream *fout);
int generate_graph(Node *root);


#endif