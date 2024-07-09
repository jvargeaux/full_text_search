#pragma once

#include <any>
#include <iomanip>
#include <iostream>
#include <string>
#include <typeinfo>
#include <unordered_map>


namespace ArgParser {

	struct Argument {
		std::vector<std::string> aliases;
		std::any value;
		Argument(std::vector<std::string> _aliases, std::any _value) : aliases(_aliases), value(_value) {}
	};

	class ArgumentsList {
		friend ArgumentsList parse_args(int argc, char** argv);
		std::unordered_map<std::string, std::shared_ptr<Argument>> argument_aliases;

		void add_argument(std::vector<std::string> aliases, std::any value) {
			std::shared_ptr<Argument> new_argument(new Argument(aliases, value));
			for (std::string alias : aliases) {
				argument_aliases[alias] = new_argument;
			}
		};

		public:
			void print_argument_value(const std::shared_ptr<Argument>& argument_ptr) {
				if (std::string *s = std::any_cast<std::string>(&(argument_ptr->value))) {
					std::cout << *s;
				}
				if (int *i = std::any_cast<int>(&(argument_ptr->value))) {
					std::cout << *i;
				}
			};
			void print_all_arguments() {
				std::cout << "\nArguments:\n";
				std::shared_ptr<Argument> argument_ptr;
				for (const std::pair<std::string, std::shared_ptr<Argument>> &pair : argument_aliases) {
					if (argument_ptr == pair.second) {
						std::cout << '\t' << std::setw(20) << std::left << pair.first << '\n';
					}
					else {
						std::cout << '\t' << std::setw(20) << std::left << pair.first << '\t';
						print_argument_value(pair.second);
						std::cout << '\n';
					}
					argument_ptr = pair.second;
				}
			};
			bool has(std::string& key) {
				return argument_aliases.find(key) != argument_aliases.end();  // C++20 can use contains()
			};
			template<typename T> T get(const std::string& key) {
				auto argument = argument_aliases.find(key);
				if (argument == argument_aliases.end()) {
					throw std::runtime_error("Argument not found");
				}
				return std::any_cast<T>((argument->second)->value);
			};
	};

	ArgumentsList parse_args(int argc, char** argv);
	void print_help();

}

