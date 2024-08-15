#pragma once

constexpr bool benchmark_oneline = true;

void run_benchmark(std::vector<std::string> build_strings, std::vector<std::string> query_strings, std::vector<size_t> num_iterations);