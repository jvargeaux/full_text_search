#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "benchmark.hpp"
#include "generate_graph.hpp"
#include "suffix_tree.hpp"


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


BenchmarkIterationResult benchmark_function(std::vector<std::tuple<size_t, size_t>> (*benchmark_function) (BenchmarkData *data, size_t query_index), BenchmarkData *data, size_t num_iterations) {
	std::vector<std::tuple<size_t, size_t>> matches {};
	BenchmarkIterationResult result;

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
		std::chrono::duration<double, std::milli> time_ms = (t2 - t1);
		std::chrono::duration<double, std::milli> time_ms_per_iter = (t2 - t1) / num_iterations;

		result.times_ms.push_back(time_ms_per_iter.count());

		// Print results
		if constexpr(benchmark_oneline) {
			std::cout << "\"" << data->query_strings[q] << "\" (" << matches.size() << " matches): "
			          << num_iterations << " iterations in " << time_ms.count() << " ms -> "
					  << time_ms_per_iter.count() << " ms/it\n";
		}
		else {
			std::chrono::duration<double> time_s_per_iter = (t2 - t1) / num_iterations;
			auto time_ms_per_iter = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1) / num_iterations;
			auto time_us_per_iter = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1) / num_iterations;
			auto time_ns_per_iter = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1) / num_iterations;

			std::cout << "\"" << data->query_strings[q] << "\" (" << matches.size() << " matches) -> "
			          << num_iterations << " iterations in " << time_ms.count() << " s\n"
			          << std::right
			          << std::setw(10) << std::fixed << std::setprecision(9) << time_s_per_iter.count() << "  s / it\n"
			          << std::setw(5) << time_ms_per_iter.count() << "       ms / it\n"
			          << std::setw(8) << time_us_per_iter.count() % 1000 << "    μs / it\n"
			          << std::setw(11) << time_ns_per_iter.count() % 1000 << " ns / it\n\n";
		}
	}

	return result;
}


BenchmarkResults run_benchmark(std::vector<std::string> build_strings, std::vector<std::string> query_strings, std::vector<size_t> num_iterations) {
	size_t suffix_tree_size = 0;
	for (const std::string& build_string : build_strings) {
		suffix_tree_size += build_string.size();
	}

	// Build suffix tree
	// std::cout << "\n\nBuilding suffix tree...\n";
	Node *root = new Node();
	build_suffix_tree_naive(build_strings, root);
	std::cout << "\n\n"
	          << "-----------------\n"
	          << "Suffix Tree Size: " << suffix_tree_size << "\n"
	          << "-----------------\n";

	// Lambda
	// auto test_fn = []() {}

	BenchmarkData data = {
		root,
		build_strings,
		query_strings
	};

	BenchmarkResults results;

	std::cout << "\nRaw sequential search\n-----\n";
	for (const size_t& iterations: num_iterations) {
		results.raw_sequential.push_back(benchmark_function(benchmark_raw_search, &data, iterations));
	}

	std::cout << "\nSuffix tree\n-----\n";
	for (const size_t& iterations: num_iterations) {
		results.suffix_tree.push_back(benchmark_function(benchmark_suffix_tree, &data, iterations));
	}

	return results;
}


int main() {
	std::vector<std::string> build_strings = {"green$", "end$"};
	std::vector<std::string> query_strings = {"en", "qwerty"};
	std::vector<size_t> num_iterations = {10, 1000};

	BenchmarkResults results = run_benchmark(build_strings, query_strings, num_iterations);
}