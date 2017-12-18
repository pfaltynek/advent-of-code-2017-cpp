#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

std::regex line_template("^Generator (A|B) starts with (\\d+)$");

bool ParseLine (std::string line, bool &genA, int &start_val){
	std::smatch sm;

	if (regex_match(line,sm, line_template)){
		genA = (sm.str(1).compare ("A") == 0);
		start_val = stoi(sm.str(2));
		return true;
	}
	return false;
}

int main(void) {
	int result1 = 0, result2 = 0, genA_start, genB_start, cnt = 0;
	std::ifstream input;
	std::string line;
	bool genA = false, genB = false;
	std::cout << "=== Advent of Code 2017 - day 15 ====" << std::endl;
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
		int x;
		cnt++;
		if (!ParseLine(line, genA, x)) {
			std::cout << "Invalid program info on line " << cnt << std::endl;
		} else {
			if (genA) {
				genA_start = x;
			} else {
				genB = true;
				genB_start = x;
			}
		}
	}

	if (!genA || !genB) {
		std::cout << "Incomplette input data" << cnt << std::endl;
		return -1;
	}

	if (input.is_open()) {
		input.close();
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
