#include <fstream>
#include <iostream>
#include <vector>

// hexagon grids implementation theory ...
// https://www.redblobgames.com/grids/hexagons/

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

HexWay GetOpositeWay(HexWay way) {
	switch (way) {
		case HexWay::N:
			return HexWay::S;
			break;
		case HexWay::S:
			return HexWay::N;
			break;
		case HexWay::NE:
			return HexWay::SW;
			break;
		case HexWay::NW:
			return HexWay::SE;
			break;
		case HexWay::SW:
			return HexWay::NE;
			break;
		case HexWay::SE:
			return HexWay::NW;
			break;
		default:
			return HexWay::N;
			break;
	}
}

void OptimizeInput(std::vector<HexWay> &input) {
	unsigned int idx = 0;
	HexWay oposite;
	bool removed = false;

	while (idx < (input.size() - 1)) {
		oposite = GetOpositeWay(input[idx]);
		removed = false;
		for (unsigned int i = idx + 1; i < input.size(); i++) {
			if (input[i] == oposite) {
				input.erase(input.begin() + i);
				input.erase(input.begin() + idx);
				break;
			}
		}
		if (!removed) {
			idx++;
		}
	}
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

int GetHexagonsDistance(int x1, int y1, int z1, int x2 = 0, int y2 = 0, int z2 = 0) {
	return (abs(x2 - x1) + abs(y2 - y1) + abs(z2 - z1)) / 2;
}

void TraceHexWays(std::vector<HexWay> input, int &result1, int &result2) {
	int x1, x2, y1, y2, z1, z2;

	x1 = y1 = z1 = x2 = y2 = z2 = result2 = 0;

	for (unsigned int i = 0; i < input.size(); i++) {
		switch (input[i]) {
			case HexWay::N:
				y1++;
				z1--;
				break;
			case HexWay::S:
				y1--;
				z1++;
				break;
			case HexWay::NE:
				x1++;
				z1--;
				break;
			case HexWay::NW:
				x1--;
				y1++;
				break;
			case HexWay::SW:
				x1--;
				z1++;
				break;
			case HexWay::SE:
				x1++;
				y1--;
				break;
		}

		result1 = GetHexagonsDistance(x1, y1, z1);
		if (result1 > result2) {
			x2 = x1;
			y2 = y1;
			z2 = z1;
			result2 = result1;
		}
	}
}


int main(void) {
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line, stream;

	std::cout << "=== Advent of Code 2017 - day 11 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	input.open("input.txt", std::ifstream::in);

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

	std::vector<HexWay> hex_ways = ParseInput(stream);
	// do not optimize due part 2 task
	// OptimizeInput(hex_ways);
	TraceHexWays(hex_ways, result1, result2);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
