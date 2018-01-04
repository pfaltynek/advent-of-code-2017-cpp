#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

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
	std::vector<std::string> output, lines, lines2, lines3;
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
	variants[input] = input;
	// h-split
	lines2 = SplitHorizontally(lines);
	variants[Join(lines2, "/")] = input;
	//v-split
	lines2 = SplitVertically(lines);
	variants[Join(lines2, "/")] = input;
	// v&h-split
	lines2 = SplitHorizontally(lines2);
	variants[Join(lines2, "/")] = input;
	// rotate right
	lines2 = RotateClockwise(lines);
	variants[Join(lines2, "/")] = input;
	lines3 = SplitHorizontally(lines2);
	variants[Join(lines3, "/")] = input;
	lines3 = SplitVertically(lines2);
	variants[Join(lines3, "/")] = input;
	// rotate left
	lines2 = RotateCounterClockwise(lines);
	variants[Join(lines2, "/")] = input;
	lines3 = SplitHorizontally(lines2);
	variants[Join(lines3, "/")] = input;
	lines3 = SplitVertically(lines2);
	variants[Join(lines3, "/")] = input;
	return true;
}

std::vector<std::string> IteratePattern(std::vector<std::string> pattern, std::map<std::string, std::vector<std::string>> rules,
										std::map<std::string, std::string> variants, unsigned int iterations) {
	std::vector<std::string> new_pattern, part;
	std::string input;
	unsigned int blocks;

	for (unsigned int i = 0; i < iterations; ++i) {
		new_pattern.clear();

		if ((pattern.size() % 2) == 0) {

			blocks = pattern.size() / 2;
			new_pattern.resize(blocks * 3);

			for (unsigned int j = 0; j < blocks; ++j) {
				for (unsigned int k = 0; k < blocks; ++k) {
					part.clear();
					part.push_back(pattern[j * 2].substr(k * 2, 2));
					part.push_back(pattern[(j * 2) + 1].substr(k * 2, 2));
					input = Join(part, "/");
					if (variants.find(input) == variants.end()) {
						// problem ???
						int z = 78;
					} else {
						input = variants[input];
						new_pattern[3 * j].append(rules[input][0]);
						new_pattern[(3 * j) + 1].append(rules[input][1]);
						new_pattern[(3 * j) + 2].append(rules[input][2]);
					}
				}
			}
		} else if ((pattern.size() % 3) == 0) {

			blocks = pattern.size() / 3;
			new_pattern.resize(blocks * 4);

			for (unsigned int j = 0; j < blocks; ++j) {
				for (unsigned int k = 0; k < blocks; ++k) {
					part.clear();
					part.push_back(pattern[j * 3].substr(k * 3, 3));
					part.push_back(pattern[(j * 3) + 1].substr(k * 3, 3));
					part.push_back(pattern[(j * 3) + 2].substr(k * 3, 3));
					input = Join(part, "/");
					if (variants.find(input) == variants.end()) {
						// problem ???
						int z = 78;
					} else {
						input = variants[input];
						new_pattern[4 * j].append(rules[input][0]);
						new_pattern[(4 * j) + 1].append(rules[input][1]);
						new_pattern[(4 * j) + 2].append(rules[input][2]);
						new_pattern[(4 * j) + 3].append(rules[input][3]);
					}
				}
			}
		} else {
			// problem???
			int z = 13;
		}

		pattern = new_pattern;
	}

	return new_pattern;
}

int CalculateOnPixels(std::vector<std::string> pattern) {
	std::string line;

	line = Join(pattern, "/");
	return std::count(line.begin(), line.end(), '#');
}

int main(void) {
	int cnt = 0, result1 = 0, result2 = 0, iterations = 0;
	std::ifstream input;
	std::string line;
	std::map<std::string, std::vector<std::string>> rules;
	std::map<std::string, std::string> variants;
	std::cout << "=== Advent of Code 2017 - day 21 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;
	std::vector<std::string> pattern, tt;

#if TEST
	input.open("input-test.txt", std::ifstream::in);
	iterations = 2;
#else
	input.open("input.txt", std::ifstream::in);
	iterations = 5;
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

	result1 = CalculateOnPixels(IteratePattern(pattern, rules, variants, iterations));


	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	iterations = 18;
	result2 = CalculateOnPixels(IteratePattern(pattern, rules, variants, iterations));

	std::cout << "Result is " << result2 << std::endl;
}
