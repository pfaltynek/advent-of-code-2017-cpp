#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

std::regex component_template("^(\\d+)\\/(\\d+)$");

class component {
  public:
	std::string desc;
	int first, second;
};

void AddEmptyListIfNeeded(std::map<int, std::map<int, component>> &components, int val) {
	if (components.find(val) == components.end()) {
		std::map<int, component> list;
		list.clear();
		components[val] = list;
	}
}

bool ParseComponent(std::string line, std::map<int, std::map<int, component>> &components, std::map<std::string, bool> &components_available) {
	std::smatch sm;

	if (std::regex_match(line, sm, component_template)) {
		component comp;
		int l, r;

		l = stoi(sm.str(1));
		r = stoi(sm.str(2));
		AddEmptyListIfNeeded(components, l);
		AddEmptyListIfNeeded(components, r);
		comp.desc = line;
		comp.first = l;
		comp.second = r;
		components[l][r] = comp;
		comp.first = r;
		comp.second = l;
		components[r][l] = comp;
		components_available[line] = true;
		return true;
	} else {
		return false;
	}
}

void FindStrongestBridge(int start, std::map<int, std::map<int, component>> &components, std::map<std::string, bool> components_available, int sum, int size,
						  unsigned int &longest, unsigned int &result1, unsigned int &result2) {

	if (components.find(start) != components.end()) {
		for (auto it = components[start].begin(); it != components[start].end(); it++) {
			int end;
			unsigned int sum_new, size_new;

			if (components_available[it->second.desc]) {
				end = it->second.second;

				components_available[it->second.desc] = false;
				sum_new = sum + start + end;
				size_new = size + 1;
				if (sum_new > result1) {
					result1 = sum_new;
				}
				if (longest < size_new) {
					longest = size_new;
					result2 = size_new;
				} else if ((longest == size_new) && (result2 < sum_new)) {
					result2 = sum_new;
				}
				FindStrongestBridge(end, components, components_available, sum_new, size_new, longest, result1, result2);

				components_available[it->second.desc] = true;
			}
		}
	}
}

int main(void) {
	int cnt = 0;
	unsigned int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::map<int, std::map<int, component>> components;
	std::map<std::string, bool> components_available;
	unsigned int longest = 0;

	std::cout << "=== Advent of Code 2017 - day 24 ====" << std::endl;
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

	components.clear();
	components_available.clear();

	while (std::getline(input, line)) {
		cnt++;
		if (!ParseComponent(line, components, components_available)) {
			std::cout << "Unable to decode grid map at line " << cnt << std::endl;
		}
	}

	if (input.is_open()) {
		input.close();
	}

	FindStrongestBridge(0, components, components_available, 0, 0, longest, result1, result2);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
