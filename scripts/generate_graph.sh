#! /bin/bash

root_dir=$(realpath `dirname $0`/..)


if [[ ! -f $root_dir/graph.gv ]]; then
    echo "No graph.gv found"
    exit 1
fi

dot -Tsvg $root_dir/graph.gv > $root_dir/graph.svg
