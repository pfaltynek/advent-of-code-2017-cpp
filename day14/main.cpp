#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

const std::string input("stpzcrnm");

std::vector<int> PrepareLengths(std::string input) {
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

std::string GetDenseHash(std::vector<unsigned char> sparse_hash) {
	unsigned int idx;
	std::string result;
	unsigned char value;

	idx = value = 0;
	result.clear();

	while (idx < sparse_hash.size()) {
		value ^= sparse_hash[idx];
		idx++;
		if (((idx % 16) == 0) || (idx == sparse_hash.size())) {
			result.append(ByteToHex(value & 0xFF));
			value = 0;
		}
	}

	return result;
}

std::vector<unsigned char> GetDenseHash(std::vector<int> sparse_hash) {
	unsigned int idx;
	std::vector<unsigned char> result;
	unsigned char value;

	idx = value = 0;
	result.clear();

	while (idx < sparse_hash.size()) {
		value ^= sparse_hash[idx];
		idx++;
		if (((idx % 16) == 0) || (idx == sparse_hash.size())) {
			result.push_back(value & 0xFF);
			value = 0;
		}
	}

	return result;
}

std::vector<unsigned char> GetKnotHash(std::string input) {
	std::vector<int> list;
	int current, skip, temp;
	std::vector<int> lengths;

	lengths = PrepareLengths(input);

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

int GetBitOnCount(std::vector<unsigned char> hash) {
	int result = 0;
	unsigned char val;

	for (unsigned int i = 0; i < hash.size(); i++) {
		val = hash[i];
		for (int j = 0; j < 8; j++) {
			if (val & 0x01) {
				result++;
			}
			val = val >> 1;
		}
	}

	return result;
}

int GetUsedSectorsCount(std::string input) {
	std::string line;
	int result = 0;
	std::vector<unsigned char> hash;

	for (int i = 0; i < 128; i++) {
		line = input + "-" + std::to_string(i);
		hash = GetKnotHash(line);
		result += GetBitOnCount(hash);
	}
	return result;
}

int main(void) {
	int result1 = 0, result2 = 0;

	std::cout << "=== Advent of Code 2017 - day 14 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST
	result1 = GetUsedSectorsCount("flqrgnkx");
#else
	result1 = GetUsedSectorsCount(input);
#endif

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
