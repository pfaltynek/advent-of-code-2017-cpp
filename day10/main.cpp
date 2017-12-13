#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define TEST1 0
#define TEST2 0

std::vector<int> Split(std::string to_split, const std::string &delimiter) {
	std::vector<int> result;
	size_t pos;

	result.clear();

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

std::string ByteToHex(unsigned char val) {
	std::string result;
	unsigned char digit;

	result.clear();

	for (unsigned int i = 0; i < 2; i++) {
		digit = val % 16;
		if (digit < 10) {
			result = ((char)(digit + '0')) + result;
		} else {
			result = ((char)(digit + 'a' - 10)) + result;
		}
		val /= 16;
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
	std::vector<int> list;
	int current, skip, temp;

	for (int i = 0; i <= max; i++) {
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

std::string GetDenseHash(std::vector<int> sparse_hash) {
	unsigned int idx;
	std::string result;
	unsigned char value;

	idx = value = 0;
	result.clear();

	while (idx < sparse_hash.size()) {
		value ^= sparse_hash[idx];
		idx++;
		if (((idx % 16) == 0) || (idx == sparse_hash.size())) {
			result.append(ByteToHex(value));
			value = 0;
		}
	}

	return result;
}

std::string GetKnotHash(std::vector<int> lengths) {
	std::vector<int> list;
	int current, skip, temp;

	for (int i = 0; i <= 255; i++) {
		list.push_back(i);
	}

	current = skip = 0;

	for (unsigned int round = 0; round < 64; round++) {
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
	}

	return GetDenseHash(list);
}

int main(void) {
	int result1 = 0, max;
	std::ifstream input;
	std::string line, stream, result2;
	std::vector<int> lengths;

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
	lengths.clear();
	result2.clear();

	while (std::getline(input, line)) {
		if (stream.size() && line.size()) {
			stream.append(",");
		}
		stream.append(line);
	}

	if (input.is_open()) {
		input.close();
	}
	lengths = Split(stream, ",");
	ProcessList(max, lengths, result1);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

#if TEST2
	lengths = GetPart2Lengths("");
	result2 = GetKnotHash(lengths);
	result1 += result2.compare("a2582a3a0e66e6e86e3812dcb672a272");

	lengths = GetPart2Lengths("AoC 2017");
	result2 = GetKnotHash(lengths);
	result1 += result2.compare("33efeb34ea91902bb2f59c9920caa6cd");

	lengths = GetPart2Lengths("1,2,3");
	result2 = GetKnotHash(lengths);
	result1 += result2.compare("3efbe78a8d82f29979031a4aa0b16a9d");
	
	lengths = GetPart2Lengths("1,2,4");
	result2 = GetKnotHash(lengths);
	result1 += result2.compare("63960835bcdc130f0b66d7ff4f6a5a8e");
#endif

	lengths = GetPart2Lengths(stream);
	result2 = GetKnotHash(lengths);

	std::cout << "Result is " << result2 << std::endl;
}
