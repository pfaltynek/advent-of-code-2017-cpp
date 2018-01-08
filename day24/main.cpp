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

bool ParseComponent(std::string line, std::map<int, std::map<int, component>> &components) {
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
		return true;
	} else {
		return false;
	}
}

void FindStrongestBridge(std::vector<component> &bridge, std::map<int, std::map<int, component>> &components, int sum, unsigned int &longest, int &result1,
						 int &result2) {
	int start;

	if (bridge.size()) {
		start = bridge.back().second;
	} else {
		start = 0;
	}

	if (components.find(start) != components.end()) {
		if (components[start].size()) {
			for (auto it = components[start].begin(); it != components[start].end(); it++) {
				std::map<int, std::map<int, component>> cn(components);
				std::vector<component> bn(bridge);
				int end = it->second.second;
				int sn;

				sn = sum + start + end;
				bn.push_back(it->second);
				cn[start].erase(end);
				cn[end].erase(start);
				if (sn > result1) {
					result1 = sn;
				}
				if (longest <= bn.size()) {
					longest = bn.size();
					result2 = sn;
				}
				FindStrongestBridge(bn, cn, sn, longest, result1, result2);
			}
		}
	}
}

int main(void) {
	int cnt = 0;
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::map<int, std::map<int, component>> components;
	std::vector<component> bridge;
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
	bridge.clear();

	while (std::getline(input, line)) {
		cnt++;
		if (!ParseComponent(line, components)) {
			std::cout << "Unable to decode grid map at line " << cnt << std::endl;
		}
	}

	if (input.is_open()) {
		input.close();
	}

	FindStrongestBridge(bridge, components, 0, longest, result1, result2);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
