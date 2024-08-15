#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include "benchmark.hpp"
#include "generate_graph.hpp"
#include "suffix_tree.hpp"


struct BenchmarkData {
	Node *root;
	std::vector<std::string> build_strings;
	std::vector<std::string> query_strings;
};


std::vector<std::tuple<size_t, size_t>> benchmark_raw_search(BenchmarkData *data, size_t query_index) {
	std::vector<std::tuple<size_t, size_t>> matches {};
	for (size_t s = 0; s < data->build_strings.size(); s++) {
		for (size_t i = 0; i < data->build_strings[s].length(); i++) {
			// If first character doesn't match, move on
			if (data->build_strings[s][i] != data->query_strings[query_index][0]) {
				continue;
			}
			// If first character matches, check length of query string
			bool match = true;
			for (size_t j = 0; j < data->query_strings[query_index].length(); j++) {
				// Break if we go out of bounds or if mismatch
				if (i + j >= data->build_strings[s].length() || data->build_strings[s][i + j] != data->query_strings[query_index][j]) {
					match = false;
					break;
				}
			}
			if (match) {
				matches.push_back({s, i});
			}
		}
	}
	return matches;
}


std::vector<std::tuple<size_t, size_t>> benchmark_suffix_tree(BenchmarkData *data, size_t query_index) {
	std::vector<std::tuple<size_t, size_t>> matches {};
	query_suffix_tree(data->root, data->build_strings, data->query_strings[query_index], &matches);
	return matches;
}


void benchmark_function(std::vector<std::tuple<size_t, size_t>> (*benchmark_function) (BenchmarkData *data, size_t query_index), BenchmarkData *data, size_t num_iterations) {
	std::vector<std::tuple<size_t, size_t>> matches {};

	for (size_t q = 0; q < data->query_strings.size(); q++) {
		// Start benchmark
		auto t1 = std::chrono::steady_clock::now();

		// Call function
		for (size_t i = 0; i < num_iterations; i++) {
			benchmark_function(data, q);
		}

		// End benchmark
		auto t2 = std::chrono::steady_clock::now();

		// Get matches
		matches = benchmark_function(data, q);

		// Print results
		if constexpr(benchmark_oneline) {
			std::chrono::duration<double, std::milli> time_ms = t2 - t1;

			std::cout << "Query: \"" << data->query_strings[q] << "\", " << num_iterations << " iterations\n"
			          << matches.size() << " matches\n"
			          << time_ms.count() << " ms\n\n";
		}
		else {
			std::chrono::duration<double> time = t2 - t1;
			// auto time_s = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);
			auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
			auto time_us = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
			auto time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);

			std::cout << "Query: \"" << data->query_strings[q] << "\", " << num_iterations << " iterations\n"
			          << matches.size() << " matches\n" << std::right
			          << "\t" << std::setw(10) << std::fixed << std::setprecision(9) << time.count() << "  s\n"
			          << "\t" << std::setw(5) << time_ms.count() << "       ms\n"
			          << "\t" << std::setw(8) << time_us.count() % 1000 << "    μs\n"
			          << "\t" << std::setw(11) << time_ns.count() % 1000 << " ns\n\n";
		}
	}
}


void run_benchmark(std::vector<std::string> build_strings, std::vector<std::string> query_strings, std::vector<size_t> num_iterations) {
	// Build suffix tree
	std::cout << "\nBuilding suffix tree (this is the slow part before benchmarking)...\n";
	Node *root = new Node();
	build_suffix_tree_naive(build_strings, root);
	std::cout << "Suffix tree built.\n";
	std::cout << "Num build strings: " << build_strings.size() << "\n\n";

	// Lambda
	// auto test_fn = []() {}

	BenchmarkData data = {
		root,
		build_strings,
		query_strings
	};

	std::cout << "\nRaw sequential search\n--\n";
	for (const size_t& iterations: num_iterations) {
		benchmark_function(benchmark_raw_search, &data, iterations);
	}

	std::cout << "\nSuffix tree\n--\n";
	for (const size_t& iterations: num_iterations) {
		benchmark_function(benchmark_suffix_tree, &data, iterations);
	}
}


int main() {
	std::vector<std::string> build_strings = {"green$", "end$"};
	std::vector<std::string> query_strings = {"qwerty"};
	std::vector<size_t> num_iterations = {10, 1000};

	run_benchmark(build_strings, query_strings, num_iterations);
}