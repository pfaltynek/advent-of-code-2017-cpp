#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

#if TEST
#define PROGRAMS_MAX 5
#else
#define PROGRAMS_MAX 16
#endif

std::regex spin_template("^s(\\d+)$");
std::regex swap_by_idx_template("^x(\\d+)\/(\\d+)$");
std::regex swap_by_name_template("^p([a-p])\/([a-p])$");

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

bool IsProgramPositionValid(size_t pos) {
	return (pos >= 0) && (pos < PROGRAMS_MAX);
}

void SwapPrograms(std::string &programs, size_t pos1, size_t pos2) {
	char temp;

	temp = programs[pos1];
	programs[pos1] = programs[pos2];
	programs[pos2] = temp;
}

std::string LetProgramsDance(std::vector<std::string> dance_moves, long long count) {
	std::string programs;
	std::smatch sm;
	size_t pos1, pos2;
	std::string first;

	programs.clear();

	for (int i = 0; i < PROGRAMS_MAX; i++) {
		programs += (char)('a' + i);
	}

	first = programs;

	for (long long j = 1; j <= count; j++) {
		for (unsigned int i = 0; i < dance_moves.size(); i++) {
			if (std::regex_match(dance_moves[i], sm, spin_template)) {
				pos1 = stoi(sm.str(1));
				if (IsProgramPositionValid(pos1)) {
					programs = programs.substr(programs.size() - pos1) + programs.substr(0, programs.size() - pos1);
				} else {
					std::cout << "Unable to dance move '" << dance_moves[i] << "'" << std::endl;
				}
			} else if (std::regex_match(dance_moves[i], sm, swap_by_idx_template)) {
				pos1 = stoi(sm.str(1));
				pos2 = stoi(sm.str(2));
				if (IsProgramPositionValid(pos1) && IsProgramPositionValid(pos2)) {
					SwapPrograms(programs, pos1, pos2);
				} else {
					std::cout << "Unable to dance move '" << dance_moves[i] << "'" << std::endl;
				}
			} else if (std::regex_match(dance_moves[i], sm, swap_by_name_template)) {
				pos1 = programs.find(sm.str(1), 0);
				pos2 = programs.find(sm.str(2), 0);
				if (IsProgramPositionValid(pos1) && IsProgramPositionValid(pos2)) {
					SwapPrograms(programs, pos1, pos2);
				} else {
					std::cout << "Unable to dance move '" << dance_moves[i] << "'" << std::endl;
				}
			} else {
				std::cout << "Unable to dance move '" << dance_moves[i] << "'" << std::endl;
			}
		}
		if (first.compare(programs) == 0) {
			long long rest;

			rest = count % j;
			j = count - rest;
		}
	}
	return programs;
}

int main(void) {
	std::string result1, result2;
	std::ifstream input;
	std::string line, stream;
	std::vector<std::string> list;

	std::cout << "=== Advent of Code 2017 - day 16 ====" << std::endl;
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

	list = Split(stream, ",");

	result1 = LetProgramsDance(list, 1);
	result2 = LetProgramsDance(list, 1000000000);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
