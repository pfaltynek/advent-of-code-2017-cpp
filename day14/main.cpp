#include <iostream>
#include <map>
#include <vector>

#define TEST 0

const std::string input("stpzcrnm");

unsigned int GetCoord(unsigned char x, unsigned char y) {
	return (x * 1000) + y;
}

unsigned char GetXCoord(unsigned int coord) {
	return coord / 1000;
}

unsigned char GetYCoord(unsigned int coord) {
	return coord % 1000;
}

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

int GetUsedSectorsCount(std::map<unsigned int, bool> &bit_map) {
	int result = 0;

	for (auto it = bit_map.begin(); it != bit_map.end(); it++) {
		if (it->second) {
			result++;
		}
	}

	return result;
}

void FillGridMap(std::string input, std::map<unsigned int, bool> &bit_map) {
	std::string line;
	std::vector<unsigned char> hash;
	unsigned char val, x, y;
	bool bit;
	unsigned int coord;

	for (int ii = 0; ii < 128; ii++) {
		line = input + "-" + std::to_string(ii);
		hash = GetKnotHash(line);

		for (unsigned int i = 0; i < hash.size(); i++) {
			val = hash[i];
			for (int j = 0; j < 8; j++) {
				x = (i * 8) + j;
				y = ii;
				coord = GetCoord(x, y);
				bit = val & 0x80;
				bit_map[coord] = bit;
				val = val << 1;
			}
		}
	}
}

int GetRegionsCount(std::map<unsigned int, bool> bit_map) {
	int result = 0;
	std::vector<unsigned int> queue;
	bool in_region = false, state;
	unsigned int coord;
	unsigned char x, y;

	while (bit_map.size()) {

		queue.push_back(bit_map.begin()->first);

		while (queue.size()) {
			coord = queue.back();
			queue.pop_back();
			x = GetXCoord(coord);
			y = GetYCoord(coord);

			if (bit_map.find(coord) == bit_map.end()) {
				state = false;
			} else {
				state = bit_map[coord];
			}

			if (state) {
				if (!in_region) {
					in_region = true;
					result++;
				}

				queue.push_back(GetCoord(x + 1, y));
				queue.push_back(GetCoord(x - 1, y));
				queue.push_back(GetCoord(x, y + 1));
				queue.push_back(GetCoord(x, y - 1));
			}

			bit_map.erase(coord);
		}
		in_region = false;
	}

	return result;
}

int main(void) {
	int result1 = 0, result2 = 0;
	std::map<unsigned int, bool> bit_map;

	std::cout << "=== Advent of Code 2017 - day 14 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	bit_map.clear();

#if TEST
	FillGridMap("flqrgnkx", bit_map);
#else
	FillGridMap(input, bit_map);
#endif

	result1 = GetUsedSectorsCount(bit_map);
	result2 = GetRegionsCount(bit_map);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
