#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

std::regex component_template("^(\\d+)\\/(\\d+)$");

void AddEmptyListIfNeeded(std::map<int, std::map<int, std::string>> &components, int val) {
	if (components.find(val) == components.end()) {
		std::map<int, std::string> list;
		list.clear();
		components[val] = list;
	}
}

bool ParseComponent(std::string line, std::map<int, std::map<int, std::string>> &components) {
	std::smatch sm;

	if (std::regex_match(line, sm, component_template)) {
		int l, r;
		l = stoi(sm.str(1));
		r = stoi(sm.str(2));
		AddEmptyListIfNeeded(components, l);
		AddEmptyListIfNeeded(components, r);
		components[l][r] = line;
		components[r][l] = line;
		return true;
	} else {
		return false;
	}
}

int main(void) {
	int cnt = 0;
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::map<int, std::map<int, std::string>> components;

	std::cout << "=== Advent of Code 2017 - day 24 ====" << std::endl;
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

	components.clear();

	while (std::getline(input, line)) {
		cnt++;
		if (!ParseComponent(line, components)) {
			std::cout << "Unable to decode grid map at line " << cnt << std::endl;
		}
	}

	if (input.is_open()) {
		input.close();
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
