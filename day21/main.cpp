#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 1

std::regex size2rule_template("^([.#]{2}\\/[.#]{2}) => ([.#]{3})\\/([.#]{3})\\/([.#]{3})$");
std::regex size3rule_template("^([.#]{3}\\/[.#]{3}\\/[.#]{3}) => ([.#]{4})\\/([.#]{4})\\/([.#]{4})\\/([.#]{4})$");

std::string Join(std::vector<std::string> strings, const std::string delimiter) {
	std::string result;

	result.clear();

	for (unsigned int i = 0; i < strings.size(); ++i) {
		if (!result.empty()) {
			result.append(delimiter);
		}
		result.append(strings[i]);
	}

	return result;
}

std::vector<std::string> Split(std::string to_split, const std::string delimiter) {
	std::vector<std::string> result;
	size_t pos;

	result.clear();

	pos = to_split.find(delimiter, 0);
	while (pos != std::string::npos) {
		if (pos) {
			result.push_back(to_split.substr(0, pos));
		}
		to_split = to_split.substr(pos + delimiter.size(), std::string::npos);

		pos = to_split.find(delimiter, 0);
	}

	if (to_split.size()) {
		result.push_back(to_split);
	}

	return result;
}

std::vector<std::string> RotateClockwise(std::vector<std::string> input) {
	std::vector<std::string> result(input);

	for (unsigned int i = 0; i < input.size(); ++i) {
		for (unsigned int j = 0; j < input.size(); ++j) {
			result[j][input.size() - i - 1] = input[i][j];
		}
	}

	return result;
}

std::vector<std::string> RotateCounterClockwise(std::vector<std::string> input) {
	std::vector<std::string> result(input);

	for (unsigned int i = 0; i < input.size(); ++i) {
		for (unsigned int j = 0; j < input.size(); ++j) {
			result[input.size() - j - 1][i] = input[i][j];
		}
	}

	return result;
}

std::vector<std::string> SplitHorizontally(std::vector<std::string> input) {
	std::vector<std::string> result(input);

	for (unsigned int i = 0; i < (input.size() / 2); ++i) {
		result[i] = input[input.size() - i - 1];
		result[input.size() - i - 1] = input[i];
	}

	return result;
}

std::vector<std::string> SplitVertically(std::vector<std::string> input) {
	std::vector<std::string> result(input);

	for (unsigned int j = 0; j < input.size(); ++j) {
		for (unsigned int i = 0; i < (input.size() / 2); ++i) {
			result[j][i] = input[j][input.size() - i - 1];
			result[j][input.size() - i - 1] = input[j][i];
		}
	}

	return result;
}

bool ParseRule(std::string line, std::map<std::string, std::vector<std::string>> &rules, std::map<std::string, std::string> &variants) {
	std::smatch sm;
	std::vector<std::string> output, lines, lines2;
	std::string input;

	output.clear();

	if (std::regex_match(line, sm, size2rule_template)) {
		input = sm.str(1);
		output.push_back(sm.str(2));
		output.push_back(sm.str(3));
		output.push_back(sm.str(4));
	} else if (std::regex_match(line, sm, size3rule_template)) {
		input = sm.str(1);
		output.push_back(sm.str(2));
		output.push_back(sm.str(3));
		output.push_back(sm.str(4));
		output.push_back(sm.str(5));
	} else {
		return false;
	}

	rules[input] = output;
	lines = Split(input, "/");

	return true;
}

std::vector<std::string> IteratePattern(std::vector<std::string> pattern, std::map<std::string, std::vector<std::string>> rules, unsigned int iterations) {
	std::vector<std::string> new_pattern;

	return new_pattern;
}

int main(void) {
	int cnt = 0, result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::map<std::string, std::vector<std::string>> rules;
	std::map<std::string, std::string> variants;
	std::cout << "=== Advent of Code 2017 - day 21 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;
	std::vector<std::string> pattern;

#if TEST
	input.open("input-test.txt", std::ifstream::in);
#else
	input.open("input.txt", std::ifstream::in);
#endif

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	rules.clear();
	pattern.clear();
	variants.clear();

	pattern.push_back(".#.");
	pattern.push_back("..#");
	pattern.push_back("###");

	while (std::getline(input, line)) {
		cnt++;
		if (!ParseRule(line, rules, variants)) {
			std::cout << "Unable to decode rule at line " << cnt << std::endl;
		}
	}

	if (input.is_open()) {
		input.close();
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
