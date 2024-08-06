#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include "generate_graph.hpp"
#include "suffix_tree.hpp"


struct BenchmarkData {
	Node *root;
	std::vector<std::string> build_strings;
	string query_string;
};


void benchmark_raw_search(BenchmarkData *data) {
	vector<int> matches {};
	for (int s = 0; s < data->build_strings.size(); s++) {
		for (int i = 0; i < data->build_strings[s].length(); i++) {
			// If first character doesn't match, move on
			if (data->build_strings[s][i] != data->query_string[0]) {
				continue;
			}
			// If first character matches, check length of query string
			bool match = true;
			for (int j = 0; j < data->query_string.length(); j++) {
				// Break if we go out of bounds or if mismatch
				if (i + j >= data->build_strings[s].length() || data->build_strings[s][i + j] != data->query_string[j]) {
					match = false;
					break;
				}
			}
			if (match) {
				matches.push_back(i);
			}
		}
	}
	// std::cout << "\nQuery result: " << '\n';
	// for (size_t m = 0; m < matches.size(); m++) {
	// 	std::cout << matches[m] << '\n';
	// }
}


void benchmark_suffix_tree(BenchmarkData *data) {
	std::vector<std::tuple<int, int>> matches {};
	query_suffix_tree(data->root, data->build_strings, data->query_string, &matches);
	// std::cout << "\nQuery result: " << '\n';
	// for (size_t m = 0; m < matches.size(); m++) {
	// 	std::cout << matches[m] << '\n';
	// }
}


void benchmark_function(void (*benchmark_function) (BenchmarkData *data), BenchmarkData *data, size_t num_iterations) {
	// Start benchmark
	auto t1 = std::chrono::high_resolution_clock::now();

	// Call function
	for (size_t i = 0; i < num_iterations; i++) {
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
	std::cout << "Finished " << num_iterations << " iterations in\n\n" << std::right
	          << "\t" << std::setw(10) << std::fixed << std::setprecision(9) << time.count() << "  s\n"
	          << "\t" << std::setw(5) << time_ms.count() << "       ms\n"
	          << "\t" << std::setw(8) << time_us.count() % 1000 << "    μs\n"
	          << "\t" << std::setw(11) << time_ns.count() % 1000 << " ns\n\n";
}


void run_benchmark(std::vector<std::string> build_strings, std::string query_string, size_t num_iterations) {
	// Build suffix tree
	std::cout << "\nBuilding suffix tree (this is the slow part before benchmarking)...\n";
	Node *root = new Node();
	build_suffix_tree_naive(build_strings, root);
	std::cout << "Suffix tree built.\n\n";

	// Lambda
	// auto test_fn = []() {}

	BenchmarkData data = {
		root,
		build_strings,
		query_string
	};

	// Benchmark
	size_t iterations = 1;

	// Print strings
	// std::cout << "\n\n";
	// std::cout << "Build strings:\n";
	// for (std::string build_string : build_strings) {
	// 	std::cout << build_string << " (" << build_string.length() << ")\n";
	// }
	// std::cout << "\nQuery string:\n" << query_string << " (" << query_string.length() << ")\n";

	std::cout << "\nBenchmarking sequential search...\n--\n";
	benchmark_function(benchmark_raw_search, &data, num_iterations);

	std::cout << "\nBenchmarking suffix tree...\n--\n";
	benchmark_function(benchmark_suffix_tree, &data, num_iterations);
}


int main() {
	std::vector<std::string> build_strings = {"green$", "end$"};
	string query_string = "en";
	size_t num_iterations = 100;

	run_benchmark(build_strings, query_string, num_iterations);
}