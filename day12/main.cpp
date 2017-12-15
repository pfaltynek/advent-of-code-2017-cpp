#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

std::regex line_template("^(\\d+) <-> (\\d+)(, \\d+)*$");
std::regex parts_template("^(\\d+) <-> (.+)$");

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

bool ParseLine(std::string line, std::map<int, std::vector<int>> &programs) {
	std::smatch sm;
	int prog;

	if (!regex_match(line, sm, line_template)) {
		return false;
	} else {
		regex_match(line, sm, parts_template);
	}

	prog = stoi(sm.str(1));

	programs[prog] = Split(sm.str(2), ", ");

	return true;
}

int GetProgGroupSize(int prg_grp_id, std::map<int, std::vector<int>> progs) {
	std::vector<int> group, queue;
	int id;

	group.clear();
	queue.clear();
	queue.push_back(prg_grp_id);

	while (queue.size()) {
		id = queue.back();
		queue.pop_back();
		if (std::find(group.begin(), group.end(), id) == group.end()) {
			group.push_back(id);
			for (unsigned int i = 0; i < progs[id].size(); i++) {
				queue.push_back(progs[id][i]);
			}
		}
	}

	return group.size();
}

int GetProgGroupsCount(std::map<int, std::vector<int>> progs) {
	std::vector<int> queue, prg_list, groups;
	std::vector<int>::iterator itv;
	int id;

	prg_list.clear();
	groups.clear();

	for (auto it = progs.begin(); it != progs.end(); it++) {
		prg_list.push_back(it->first);
	}

	queue.clear();

	while (prg_list.size()) {
		queue.push_back(prg_list[0]);
		groups.push_back(prg_list[0]);
		prg_list.erase(prg_list.begin());

		while (queue.size()) {
			id = queue.back();
			queue.pop_back();
			itv = std::find(prg_list.begin(), prg_list.end(), id);
			if (itv != prg_list.end()) {
				prg_list.erase(itv);
			}
			for (unsigned int i = 0; i < progs[id].size(); i++) {
				if (std::find(prg_list.begin(), prg_list.end(), progs[id][i]) != prg_list.end()) {
					if (std::find(queue.begin(), queue.end(), progs[id][i]) == queue.end()) {
						queue.push_back(progs[id][i]);
					}
				}
			}
		}
	}

	return groups.size();
}

int main(void) {
	int result1 = 0, result2 = 0, cnt = 0;
	std::ifstream input;
	std::string line;
	std::map<int, std::vector<int>> programs;

	std::cout << "=== Advent of Code 2017 - day 12 ====" << std::endl;
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
		cnt++;
		if (!ParseLine(line, programs)) {
			std::cout << "Invalid program info on line " << cnt << std::endl;
		}
	}

	if (input.is_open()) {
		input.close();
	}

	result1 = GetProgGroupSize(0, programs);
	result2 = GetProgGroupsCount(programs);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
