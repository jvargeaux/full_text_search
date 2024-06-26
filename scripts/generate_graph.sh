#! /bin/bash

root_dir=~/Dev/Professional/Ayo/full_text_search

cd $root_dir
# make graph
# bin/generate_graph
dot -Tsvg $root_dir/graph.gv > $root_dir/graph.svg