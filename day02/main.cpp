#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#define TEST 0

int GetLineChecksum(std::vector<int> line) {
	return (*(line.begin())) - (*(line.rbegin()));
}

int GetEvenlyDivisibleValues(std::vector<int> line) {
	int result = 0, a, b;

	for (int i = 0; i < line.size(); i++) {
		a = line[i];
		for (int j = i + 1; j < line.size(); j++) {
			b = line[j];
			if (b == 0) {
				continue;
			}
			if (((a / b) > 1) && ((a % b) == 0)) {
				return a / b;
			}
		}
	}
}

std::vector<int> SplitSpreadSheetLineContent(std::string line) {
	int num;
	std::stringstream ss;
	std::string temp;
	std::vector<int> result;

	result.clear();
	ss << line;
	while (!ss.eof()) {
		ss >> temp;
		if (std::stringstream(temp) >> num) {
			result.push_back(num);
		}
	}

	std::sort(result.rbegin(), result.rend());

	return result;
}

int main(void) {
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::vector<std::vector<int>> spread_sheet;

	std::cout << "=== Advent of Code 2017 - day 2 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	spread_sheet.clear();
	while (std::getline(input, line)) {
		spread_sheet.push_back(SplitSpreadSheetLineContent(line));
	}

	if (input.is_open()) {
		input.close();
	}

#if TEST
	result1 = GetLineChecksum(SplitSpreadSheetLineContent("5 1 9 5"));
	result1 = GetLineChecksum(SplitSpreadSheetLineContent("7 5 3"));
	result1 = GetLineChecksum(SplitSpreadSheetLineContent("2 4 6 8"));
	result1 = 0;

	result2 += GetEvenlyDivisibleValues(SplitSpreadSheetLineContent("5 9 2 8"));
	result2 += GetEvenlyDivisibleValues(SplitSpreadSheetLineContent("9 4 7 3"));
	result2 += GetEvenlyDivisibleValues(SplitSpreadSheetLineContent("3 8 6 5"));
	result2 = 0;
#endif

	for (auto it = spread_sheet.begin(); it != spread_sheet.end(); it++) {
		result1 += GetLineChecksum(*it);
		result2 += GetEvenlyDivisibleValues(*it);
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
