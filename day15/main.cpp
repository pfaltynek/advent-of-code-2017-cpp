#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

const long long factorA = 16807ll;
const long long factorB = 48271ll;
const long long divider = 2147483647ll;

std::regex line_template("^Generator (A|B) starts with (\\d+)$");

bool ParseLine(std::string line, bool &genA, int &start_val) {
	std::smatch sm;

	if (regex_match(line, sm, line_template)) {
		genA = (sm.str(1).compare("A") == 0);
		start_val = stoi(sm.str(2));
		return true;
	}
	return false;
}

long long GetPairsCount(long long rounds, int startA, int startB) {
	int a, b;
	long long result = 0;

	a = startA;
	b = startB;

	for (long long i = 0; i < rounds; i++) {
		a = (a * factorA) % divider;
		b = (b * factorB) % divider;

		if ((a & 0xFFFF) == (b & 0xFFFF)) {
			result++;
		}
	}

	return result;
}

long long GetPairs2Count(long long rounds, int startA, int startB) {
	int a, b;
	long long result = 0, cnt = 0;

	a = startA;
	b = startB;

	for (long long i = 0; i < rounds; i++) {
		do {
			a = (a * factorA) % divider;
		} while (a % 4);
		do {
			b = (b * factorB) % divider;
		} while (b % 8);

		if (((a % 4) == 0) && ((b % 8) == 0)) {
			if ((a & 0xFFFF) == (b & 0xFFFF)) {
				result++;
			}
		}
	}

	return result;
}

int main(void) {
	int genA_start, genB_start, cnt = 0;
	long long result1 = 0, result2 = 0;
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
		bool y;
		cnt++;
		if (!ParseLine(line, y, x)) {
			std::cout << "Invalid program info on line " << cnt << std::endl;
		} else {
			if (y) {
				genA = true;
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

	result1 = GetPairsCount(40000000ll, genA_start, genB_start);
	result2 = GetPairs2Count(5000000ll, genA_start, genB_start);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
