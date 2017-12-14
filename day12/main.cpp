#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 1

std::regex line_template("^(\\d+) <-> (\\d+)(, \\d+)*$");
std::regex parts_template("^(\\d+) <-> (.+)$");

bool ParseLine(std::string line, std::map<int, std::vector<int>> &programs) {
	std::smatch sm;
	int prog;
	
	if (!regex_match(line, sm, line_template)) {
		return false;
	}

	return true;
}

int main(void) {
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::map<int, std::vector<int>> programs;

	std::cout << "=== Advent of Code 2017 - day 12 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST
	input.open("input-test.txt", std::ifstream::in);
#else
	input.open("input.txt", std::ifstream::in);
#endif

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		if (ParseLine(line, programs)) {
		}
	}

	if (input.is_open()) {
		input.close();
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
