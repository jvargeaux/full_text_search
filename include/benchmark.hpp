#pragma once

#include "suffix_tree.hpp"

constexpr bool benchmark_oneline = true;


struct BenchmarkData {
	Node *root;
	std::vector<std::string> build_strings;
	std::vector<std::string> query_strings;
};

struct BenchmarkIterationResult {
	// std::vector<std::tuple<size_t, size_t>> matches;
	std::vector<double> times_ms;
};

struct BenchmarkResults {
	std::vector<BenchmarkIterationResult> raw_sequential;
	std::vector<BenchmarkIterationResult> suffix_tree;
};

BenchmarkResults run_benchmark(const std::vector<std::string>& build_strings, const std::vector<std::string>& query_strings, std::vector<size_t> num_iterations);