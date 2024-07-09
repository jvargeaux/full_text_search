#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include "generate_graph.hpp"
#include "suffix_tree.hpp"


struct BenchmarkData {
	Node *root;
	string original_str;
	string query_string;
};


void benchmark_raw_search(BenchmarkData *data) {
	vector<int> matches {};
	for (int i = 0; i < data->original_str.length(); i++) {
		// If first character doesn't match, move on
		if (data->original_str[i] != data->query_string[0]) {
			continue;
		}
		// If first character matches, check length of query string
		bool match = true;
		for (int j = 0; j < data->query_string.length(); j++) {
			// Break if we go out of bounds or if mismatch
			if (i + j >= data->original_str.length() || data->original_str[i + j] != data->query_string[j]) {
				match = false;
				break;
			}
		}
		if (match) {
			matches.push_back(i);
		}
	}
	// std::cout << "\nQuery result: " << '\n';
	// for (size_t m = 0; m < matches.size(); m++) {
	// 	std::cout << matches[m] << '\n';
	// }
}


void benchmark_suffix_tree(BenchmarkData *data) {
	vector<int> matches {};
	query_suffix_tree(data->root, data->original_str, data->query_string, &matches);
	// std::cout << "\nQuery result: " << '\n';
	// for (size_t m = 0; m < matches.size(); m++) {
	// 	std::cout << matches[m] << '\n';
	// }
}


void benchmark(void (*benchmark_function) (BenchmarkData *data), BenchmarkData *data, size_t iterations) {
	// Start benchmark
	auto t1 = std::chrono::high_resolution_clock::now();

	// Call function
	for (size_t i = 0; i < iterations; i++) {
		benchmark_function(data);
	}

	// End benchmark
	auto t2 = std::chrono::high_resolution_clock::now();

	// Format time
	std::chrono::duration<double> time = t2 - t1;
	auto time_s = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);
	auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	auto time_us = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
	auto time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);

	// Print results
	std::cout << "Finished " << iterations << " iterations in\n\n" << std::right
	          << "\t" << std::setw(10) << std::fixed << std::setprecision(9) << time.count() << "  s\n"
	          << "\t" << std::setw(5) << time_ms.count() << "       ms\n"
	          << "\t" << std::setw(8) << time_us.count() % 1000 << "    μs\n"
	          << "\t" << std::setw(11) << time_ns.count() % 1000 << " ns\n\n";
}


int main() {
	// string tree_str1 = "This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe. This is a video about a please like and subscribe.$";
	string tree_str1 = "greennerdgreen$";
	string query_string = "asfd";

	// Build suffix tree
	Node *root = new Node();
	build_suffix_tree_naive(tree_str1, root);
	generate_graph(tree_str1, root, "graph.gv");

	// Lambda
	// auto test_fn = []() {}

	BenchmarkData data = {
		root,
		tree_str1,
		query_string
	};

	// Benchmark
	size_t iterations = 1;

	std::cout << "\n\n";
	std::cout << "DB string length: " << tree_str1.length() << '\n';
	std::cout << "Query string length: " << query_string.length() << '\n';

	std::cout << "\nBenchmarking sequential search...\n--\n";
	benchmark(benchmark_raw_search, &data, iterations);

	std::cout << "\nBenchmarking suffix tree...\n--\n";
	benchmark(benchmark_suffix_tree, &data, iterations);

	return 0;
}
