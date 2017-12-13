#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

typedef enum { N, S, NE, NW, SE, SW } HexWay;

bool IsHexWay(std::string input, HexWay &hex_way) {
	if (input.compare("n") == 0) {
		hex_way = HexWay::N;
		return true;
	}
	if (input.compare("s") == 0) {
		hex_way = HexWay::S;
		return true;
	}
	if (input.compare("nw") == 0) {
		hex_way = HexWay::NW;
		return true;
	}
	if (input.compare("ne") == 0) {
		hex_way = HexWay::NE;
		return true;
	}
	if (input.compare("se") == 0) {
		hex_way = HexWay::SE;
		return true;
	}
	if (input.compare("sw") == 0) {
		hex_way = HexWay::SW;
		return true;
	}

	return false;
}

std::vector<HexWay> ParseInput(std::string input) {
	std::vector<HexWay> result;
	size_t pos;
	std::string delimiter(",");
	HexWay hex_way;

	result.clear();

	pos = input.find(delimiter, 0);
	while (pos != std::string::npos) {
		if (pos) {
			if (IsHexWay(input.substr(0, pos), hex_way)) {
				result.push_back(hex_way);
			}
		}
		input = input.substr(pos + delimiter.size(), std::string::npos);

		pos = input.find(delimiter, 0);
	}

	if (input.size()) {
		if (IsHexWay(input, hex_way)) {
			result.push_back(hex_way);
		}
	}

	return result;
}

int main(void) {
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line, stream;

	std::cout << "=== Advent of Code 2017 - day 11 ====" << std::endl;
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

	stream.clear();

	while (std::getline(input, line)) {
		if (stream.size() && line.size()) {
			stream.append(",");
		}
		stream.append(line);
	}

	if (input.is_open()) {
		input.close();
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
