#! /bin/bash

root_dir=$(realpath `dirname $0`/..)


if [[ $1 == "--debug" ]]; then
	if [[ ! -f $root_dir/graph/debug/svg/ ]]; then
		mkdir -p $root_dir/graph/debug/svg/
	fi

	rm -rf $root_dir/graph/debug/svg/*

	echo -e "\nGenerating step graphs..."
	for i in $root_dir/graph/debug/*.gv;
	do
		echo $(basename $i .gv)
		dot -Tsvg $i > $root_dir/graph/debug/svg/$(basename $i .gv).svg
	done
	echo "Done."
else
	if [[ ! -f $root_dir/graph.gv ]]; then
		echo "No graph.gv found"
		exit 1
	fi
	dot -Tsvg $root_dir/graph.gv > $root_dir/graph/graph.svg
fi
