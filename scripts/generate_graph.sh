#! /bin/bash

root_dir=$(realpath `dirname $0`/..)


if [[ ! -f $root_dir/graph.gv ]]; then
    echo "No graph.gv found"
    exit 1
fi

if [[ $1 == "-debug" ]]; then
	if [[ ! -f $root_dir/graph/debug/svg/ ]]; then
		mkdir -p $root_dir/graph/debug/svg/
		echo "Created debug svg directory."
	fi

	echo "Generating step graphs..."
	for i in $root_dir/graph/debug/gv/*.gv;
	do
		echo $(basename $i .gv)
		dot -Tsvg $i > $root_dir/graph/debug/svg/$(basename $i .gv).svg
	done
	echo "Done."
fi
