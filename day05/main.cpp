#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

int DecodeJumpOffset(std::string line) {
	std::stringstream ss;
	int num;

	ss << line;
	ss >> num;

	return num;
}

int JumpsSimulationPart1(std::vector<int> jumps) {
	int cnt = 0, jump;
	unsigned int idx = 0;

	while ((idx >= 0) && (idx < jumps.size())) {
		jump = jumps[idx];
		jumps[idx] = jump + 1;
		idx += jump;
		cnt++;
	}

	return cnt;
}

int JumpsSimulationPart2(std::vector<int> jumps) {
	int cnt = 0, jump;
	unsigned int idx = 0;

	while ((idx >= 0) && (idx < jumps.size())) {
		jump = jumps[idx];
		if (jump >= 3) {
			jumps[idx] = jump - 1;
		} else {
			jumps[idx] = jump + 1;
		}
		idx += jump;
		cnt++;
	}

	return cnt;
}

int main(void) {
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::vector<int> jumps;

	std::cout << "=== Advent of Code 2017 - day 5 ====" << std::endl;
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

	jumps.clear();
	while (std::getline(input, line)) {
		jumps.push_back(DecodeJumpOffset(line));
	}

	if (input.is_open()) {
		input.close();
	}

	result1 = JumpsSimulationPart1(jumps);
	result2 = JumpsSimulationPart2(jumps);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
