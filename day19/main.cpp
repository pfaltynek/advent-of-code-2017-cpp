#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

int GetCoord(int row, int col) {
	return (1000 * row) + col;
}

int GetRowCoord(int coord) {
	return coord / 1000;
}

int GetColCoord(int coord) {
	return coord % 1000;
}

void BuildTubesMap(std::string line, std::map<int, char> &tubes, int &start_coord, int row_idx) {
	size_t pos;
	int coord;
	bool finished = false;

	pos = 0;
	while (!finished) {
		pos = line.find_first_not_of(' ', pos);
		if (pos == std::string::npos) {
			finished = true;
		} else {
			coord = GetCoord(row_idx, pos);
			if (!tubes.size()) {
				start_coord = coord;
			}
			tubes[coord] = line[pos];
			pos++;
		}
	}
}

void ExploreTubes(const int start_coord, std::map<int, char> tubes, std::string &result1, int&result2) {
	char way = 0, way1, way2; // 0 - down, 1 - right, 2 - up, 3 - left
	int coord, r, c, turn1, turn2;
	bool finished = false;

	result1.clear();
	result2 = 0;
	coord = start_coord;

	while (!finished) {
		r = GetRowCoord(coord);
		c = GetColCoord(coord);

		if (tubes[coord] == '+') {
			switch (way) {
				case 0: // down
				case 2: // up
					turn1 = GetCoord(r, c - 1);
					way1 = 3; // left
					turn2 = GetCoord(r, c + 1);
					way2 = 1; // right
					break;
				case 1: // right
				case 3: // left
					turn1 = GetCoord(r - 1, c);
					way1 = 2; // up
					turn2 = GetCoord(r + 1, c);
					way2 = 0; // down
					break;
			}
			if (tubes.find(turn1) != tubes.end()) {
				coord = turn1;
				way = way1;
			} else if (tubes.find(turn2) != tubes.end()) {
				coord = turn2;
				way = way2;
			} else {
				finished = true;
			}
		} else {
			if ((tubes[coord] >= 'A') && (tubes[coord] <= 'Z')) {
				result1 += tubes[coord];
			}

			switch (way) {
				case 0: // down
					coord = GetCoord(r + 1, c);
					break;
				case 1: // right
					coord = GetCoord(r, c + 1);
					break;
				case 2: // up
					coord = GetCoord(r - 1, c);
					break;
				case 3: // left
					coord = GetCoord(r, c - 1);
					break;
			}

			finished = (tubes.find(coord) == tubes.end());
		}
		result2++;
	}
}

int main(void) {
	int cnt = 0, result2 = 0;
	std::ifstream input;
	std::string line, result1;
	std::map<int, char> tubes;
	std::cout << "=== Advent of Code 2017 - day 19 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;
	int start_coord;

#if TEST
	input.open("input-test.txt", std::ifstream::in);
#else
	input.open("input.txt", std::ifstream::in);
#endif

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	tubes.clear();

	while (std::getline(input, line)) {
		BuildTubesMap(line, tubes, start_coord, cnt);
		cnt++;
	}

	if (input.is_open()) {
		input.close();
	}

	ExploreTubes(start_coord, tubes, result1, result2);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
