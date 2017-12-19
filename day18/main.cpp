#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

std::regex spin_template("^s(\\d+)$");
std::regex swap_by_idx_template("^x(\\d+)\\/(\\d+)$");
std::regex swap_by_name_template("^p([a-p])\\/([a-p])$");

typedef enum { SND, SET, ADD, MUL, MOD, RCV, JGZ } InstType;

class {
  public:
	InstType inst;
	int val1, val2;
	char reg1, reg2;
	bool arg1value, arg2value;
} Instruction;

std::vector<std::string> Split(std::string to_split, const std::string &delimiter) {
	std::vector<std::string> result;
	size_t pos;

	result.clear();

	pos = to_split.find(delimiter, 0);
	while (pos != std::string::npos) {
		if (pos) {
			result.push_back(to_split.substr(0, pos));
		}
		to_split = to_split.substr(pos + delimiter.size(), std::string::npos);

		pos = to_split.find(delimiter, 0);
	}

	if (to_split.size()) {
		result.push_back(to_split);
	}

	return result;
}

int main(void) {
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line, stream;
	std::vector<std::string> list;

	std::cout << "=== Advent of Code 2017 - day 18 ====" << std::endl;
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
	list.clear();

	while (std::getline(input, line)) {
		stream.append(line);
		stream.append(",");
	}

	if (input.is_open()) {
		input.close();
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
