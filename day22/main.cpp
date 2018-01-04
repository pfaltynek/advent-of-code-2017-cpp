#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 1

std::string EncodePosition(long long x, long long y) {
	std::string result;

	result.clear();
	result = std::to_string(x);
	result.append("|");
	result.append(std::to_string(y));

	return result;
}

bool ParseGrid(std::vector<std::string> grid_input, std::map<std::string, char> &grid, int &line) {
	unsigned int size;
	size = grid_input.size();

	grid.clear();

	for (unsigned int i = 0; i < size; ++i) {
		if (grid_input[i].size() != size) {
			line = i + 1;
			return false;
		}
		for (unsigned int j = 0; j < size; ++j) {
			if (grid_input[i][j] == '#') {
				grid[EncodePosition(j, i)] = '#';
			}
		}
	}

	return true;
}

int CalculateInfections(std::map<std::string, char> grid, int grid_size, unsigned int bursts, bool is_part1) {
	int result = 0, xdiff, ydiff;
	long long x, y;
	unsigned char facing = 0; // up, 1 => right, 2 => down, 3 => left
	std::string pos;
	x = y = grid_size / 2;

	for (unsigned int i = 0; i < bursts; ++i) {
		pos = EncodePosition(x, y);

		if (grid.find(pos) == grid.end()) {
			grid[pos] = '.';
		}

		switch (grid[pos]) {
			case '#':
				if (is_part1) {
					grid[pos] = '.';
				} else {
					grid[pos] = 'F';
				}

				switch (facing) { // turning to right
					case 0:		  // up
						xdiff = 1;
						ydiff = 0;
						facing = 1;
						break;
					case 1: // right
						xdiff = 0;
						ydiff = 1;
						facing = 2;
						break;
					case 2: // down
						xdiff = -1;
						ydiff = 0;
						facing = 3;
						break;
					case 3: // left
						xdiff = 0;
						ydiff = -1;
						facing = 0;
						break;
					default:
						xdiff = ydiff = 0;
						break;
				}
				break;
			case '.':
				if (is_part1) {
					grid[pos] = '#';
					result++;
				} else {
					grid[pos] = 'W';
				}

				switch (facing) { // turning to left
					case 0:		  // up
						xdiff = -1;
						ydiff = 0;
						facing = 3;
						break;
					case 1: // right
						xdiff = 0;
						ydiff = -1;
						facing = 0;
						break;
					case 2: // down
						xdiff = 1;
						ydiff = 0;
						facing = 1;
						break;
					case 3: // left
						xdiff = 0;
						ydiff = 1;
						facing = 2;
						break;
					default:
						xdiff = ydiff = 0;
						break;
				}
				break;
			case 'W':
				if (is_part1) {
					// not possible
					int zzzz = 111;
					break;
				} else {
					grid[pos] = '#';
				}

				switch (facing) { // continue in previous way
					case 0:		  // up
						xdiff = 0;
						ydiff = -1;
						break;
					case 1: // right
						xdiff = 1;
						ydiff = 0;
						break;
					case 2: // down
						xdiff = 0;
						ydiff = 1;
						break;
					case 3: // left
						xdiff = -1;
						ydiff = 0;
						break;
					default:
						xdiff = ydiff = 0;
						break;
				}
				break;
			case 'F':
				if (is_part1) {
					// not possible
					int zzzz = 111;
					break;
				} else {
					grid[pos] = '.';
				}

				switch (facing) { // reverse way
					case 0:		  // up
						xdiff = 0;
						ydiff = 1;
						break;
					case 1: // right
						xdiff = -1;
						ydiff = 0;
						break;
					case 2: // down
						xdiff = 0;
						ydiff = -1;
						break;
					case 3: // left
						xdiff = 1;
						ydiff = 0;
						break;
					default:
						xdiff = ydiff = 0;
						break;
				}
				facing = (facing + 2) % 4;
				break;
			default:
				// problem?
				int xx = 115;
				break;
		}

		x += xdiff;
		y += ydiff;
	}

	return result;
}

int main(void) {
	int cnt = 0, result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::vector<std::string> grid_input;
	std::map<std::string, char> grid;

	std::cout << "=== Advent of Code 2017 - day 22 ====" << std::endl;
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

	grid_input.clear();

	while (std::getline(input, line)) {
		grid_input.push_back(line);
	}

	if (!ParseGrid(grid_input, grid, cnt)) {
		std::cout << "Unable to decode grid map at line " << cnt << std::endl;
	}

	if (input.is_open()) {
		input.close();
	}
/*
#if TEST
	result1 = CalculateInfections(grid, grid_input.size(), 7, true);
	result1 = CalculateInfections(grid, grid_input.size(), 70, true);
#endif

	result1 = CalculateInfections(grid, grid_input.size(), 10000, true);
*/
	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = CalculateInfections(grid, grid_input.size(), 10000, false);

	std::cout << "Result is " << result2 << std::endl;
}
