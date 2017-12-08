#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

void DecodeBlocks(std::vector<int> &blocks, std::string line) {
	std::stringstream ss;
	std::string temp;
	int num;

	ss << line;
	while (!ss.eof()) {
		ss >> temp;
		if (std::stringstream(temp) >> num) {
			blocks.push_back(num);
		}
	}
}

std::string GetBlocksState(std::vector<int> blocks) {
	std::string result;

	result.clear();

	for (unsigned int i = 0; i < blocks.size(); i++) {
		result.append(std::to_string(blocks[i]));
		result.append("|");
	}

	return result;
}

void ReallocationRoutine(std::vector<int> blocks, int &part1result, int &part2result) {
	int max, max_idx, idx;
	std::vector<std::string> history;
	bool have_seen = false, have_seen_2nd = false;
	std::string state, have_seen_state;
	std::vector<std::string>::iterator it;

	part1result = part2result = 0;
	have_seen_state.clear();
	history.clear();
	history.push_back(GetBlocksState(blocks));
	while (!have_seen_2nd) {
		max = INT_MIN;
		max_idx = -1;
		for (unsigned int i = 0; i < blocks.size(); i++) {
			if (blocks[i] > max) {
				max_idx = i;
				max = blocks[i];
			}
		}

		if (max_idx < 0) {
			return;
		}

		blocks[max_idx] = 0;
		idx = max_idx;
		for (int i = 0; i < max; i++) {
			idx++;
			idx %= blocks.size();
			blocks[idx]++;
		}
		state = GetBlocksState(blocks);
		if (!have_seen) {
			part1result++;
			it = std::find(history.begin(), history.end(), state);
			if (it != history.end()) {
				have_seen = true;
				have_seen_state = state;
			} else {
				history.push_back(state);
			}
		} else {
			part2result++;
			if (state.compare(have_seen_state) == 0) {
				have_seen_2nd = true;
			}
		}
	}
}

int main(void) {
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line, temp;
	std::vector<int> blocks;

	std::cout << "=== Advent of Code 2017 - day 6 ====" << std::endl;
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

	blocks.clear();
	line.clear();
	while (std::getline(input, temp)) {
		line.append("\t");
		line.append(temp);
	}

	DecodeBlocks(blocks, line);

	if (input.is_open()) {
		input.close();
	}

	ReallocationRoutine(blocks, result1, result2);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
