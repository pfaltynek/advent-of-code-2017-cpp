#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 1

int GetCoord(int row, int col) {
	return (1000 * row) + col;
}

int GetRowCoord(int coord){
	return coord / 1000;
}

int GetColCoord(int coord){
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

void ExploreTubes(const int start_coord, const std::map<int, char> &tubes, std::string &result) {
	result.clear();
	char way = 0; //0 - down, 1 - right, 2 - up, 3 - left
	int coord, r, c;

	
}

int main(void) {
	int cnt = 0, result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
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

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
