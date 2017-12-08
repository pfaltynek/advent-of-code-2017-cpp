#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define TEST 1

bool IsPassWordValidPart1(std::vector<std::string> passwords) {
	for (int i = 0; i < passwords.size(); i++) {
		for (int j = i + 1; j < passwords.size(); j++) {
			if (passwords[i].compare(passwords[j]) == 0) {
				return false;
			}
		}
	}

	return true;
}

bool IsPassWordValidPart2(std::vector<std::string> passwords) {
	std::string p1, p2;

	for (int i = 0; i < passwords.size(); i++) {
		for (int j = i + 1; j < passwords.size(); j++) {
			p1 = passwords[i];
			p2 = passwords[j];
			std::sort(p1.begin(), p1.end());
			std::sort(p2.begin(), p2.end());
			if (p1.compare(p2) == 0) {
				return false;
			}
		}
	}

	return true;
}

std::vector<std::string> SplitPasswordParts(std::string line) {
	std::stringstream ss;
	std::string temp;
	std::vector<std::string> result;

	result.clear();
	ss << line;
	while (!ss.eof()) {
		ss >> temp;
		result.push_back(temp);
	}

	return result;
}

int main(void) {
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::vector<std::vector<std::string>> pass_words;

	std::cout << "=== Advent of Code 2017 - day 4 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	pass_words.clear();
	while (std::getline(input, line)) {
		pass_words.push_back(SplitPasswordParts(line));
	}

	if (input.is_open()) {
		input.close();
	}

#if TEST
	result1 = IsPassWordValidPart1(SplitPasswordParts("aa bb cc dd ee")) ? 1 : 0;
	result1 = IsPassWordValidPart1(SplitPasswordParts("aa bb cc dd aa")) ? 2 : 0;
	result1 = IsPassWordValidPart1(SplitPasswordParts("aa bb cc dd aaa")) ? 3 : 0;
	result1 = 0;

	result2 = IsPassWordValidPart2(SplitPasswordParts("abcde fghij")) ? 1 : 0;
	result2 = IsPassWordValidPart2(SplitPasswordParts("abcde xyz ecdab")) ? 2 : 0;
	result2 = IsPassWordValidPart2(SplitPasswordParts("a ab abc abd abf abj")) ? 3 : 0;
	result2 = IsPassWordValidPart2(SplitPasswordParts("iiii oiii ooii oooi oooo")) ? 1 : 0;
	result2 = IsPassWordValidPart2(SplitPasswordParts("oiii ioii iioi iiio")) ? 2 : 0;
	result2 = 0;
#endif

	for (auto it = pass_words.begin(); it != pass_words.end(); it++) {
		result1 += IsPassWordValidPart1(*it) ? 1 : 0;
		result2 += IsPassWordValidPart2(*it) ? 1 : 0;
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
