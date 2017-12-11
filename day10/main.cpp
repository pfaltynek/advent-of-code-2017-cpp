#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

std::vector<int> Split(std::string to_split, const std::string &delimiter) {
	std::vector<int> result;
	size_t pos;

	pos = to_split.find(delimiter, 0);
	while (pos != std::string::npos) {
		if (pos) {
			result.push_back(stoi(to_split.substr(0, pos)));
		}
		to_split = to_split.substr(pos + delimiter.size(), std::string::npos);

		pos = to_split.find(delimiter, 0);
	}

	if (to_split.size()) {
		result.push_back(stoi(to_split));
	}

	return result;
}

std::vector<int> GetPart2Lengths(std::string input) {
	std::vector<int> result;

	result.clear();
	for (unsigned int i = 0; i < input.size(); i++) {
		result.push_back((int)input[i]);
	}

	result.push_back(17);
	result.push_back(31);
	result.push_back(73);
	result.push_back(47);
	result.push_back(23);

	return result;
}

void ProcessList(int max, std::vector<int> lengths, int &result1) {
	std::vector<int> list; //	(max);
	int current, skip, temp;

	for (int i = 0; i <= max; i++) {
		// list[i] = i;
		list.push_back(i);
	}

	current = skip = result1 = 0;

	for (unsigned int i = 0; i < lengths.size(); i++) {
		int len;

		len = lengths[i];
		for (int j = 0; j < len / 2; j++) {
			int from, to;

			from = (current + j) % list.size();
			to = (current + len - j - 1) % list.size();
			temp = list[from];
			list[from] = list[to];
			list[to] = temp;
		}

		current += (lengths[i] + skip) % list.size();
		skip++;
	}

	if (list.size() >= 2) {
		result1 = list[0] * list[1];
	}
}

int main(void) {
	int result1 = 0, result2 = 0, max;
	std::ifstream input;
	std::string line, stream;

	std::cout << "=== Advent of Code 2017 - day 10 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST
	input.open("input-test.txt", std::ifstream::in);
	max = 4;
#else
	input.open("input.txt", std::ifstream::in);
	max = 255;
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

	std::vector<int> lengths = Split(stream, ",");

	ProcessList(max, lengths, result1);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
