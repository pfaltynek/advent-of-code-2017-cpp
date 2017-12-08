#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

class ProgInfo {
  public:
	std::string name;
	int weight, leaves_weight;
	std::map<std::string, ProgInfo> leaves;
};

class BranchInfo {
  public:
	std::string name;
	int weight;
	std::vector<std::string> leaves_names;
};

std::vector<std::string> Split(std::string to_split, const std::string &delimiter) {
	std::vector<std::string> result;
	size_t pos;

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

std::map<std::string, ProgInfo> BuildProgramsTree(std::vector<std::string> programs) {
	std::map<std::string, ProgInfo> tree;
	std::vector<BranchInfo> branches;
	unsigned int idx = 0;
	std::smatch sm;
	std::regex leave_template("^([a-z]+) \\((\\d+)\\)$");
	std::regex branch_template("^([a-z]+) \\((\\d+)\\)( -> )([a-z]+)(, [a-z]+)*$");
	std::regex leaves_template("^.* -> (.*)$");

	tree.clear();
	branches.clear();

	while (idx < programs.size()) {
		if (regex_match(programs[idx], sm, leave_template)) {
			ProgInfo pi;

			pi.name = sm.str(1);
			pi.weight = stoi(sm.str(2));
			pi.leaves_weight = 0;
			tree[pi.name] = pi;
			programs.erase(programs.begin() + idx);
		} else {
			idx++;
		}
	}

	for (unsigned int i = 0; i < programs.size(); i++) {
		if (regex_match(programs[i], sm, branch_template)) {
			BranchInfo bi;
			std::string ls;

			bi.name = sm.str(1);
			bi.weight = stoi(sm.str(2));
			regex_match(programs[i], sm, leaves_template);
			ls = sm.str(1);
			bi.leaves_names = Split(ls, ", ");
			branches.push_back(bi);
		}
	}

	idx = 0;
	while (branches.size()) {
		bool branch_complete = true;
		
		idx %= branches.size();

		for (unsigned int i = 0; i < branches[idx].leaves_names.size(); i++) {
			std::map<std::string, ProgInfo>::iterator it;

			it = tree.find(branches[idx].leaves_names[i]);
			if (it == tree.end()) {
				branch_complete = false;
				break;
			}
		}
		if (branch_complete) {
			ProgInfo pi;

			pi.name = branches[idx].name;
			pi.weight = branches[idx].weight;
			pi.leaves.clear();
			pi.leaves_weight = 0;

			for (unsigned int i = 0; i < branches[idx].leaves_names.size(); i++) {
				std::string n;
				n = branches[idx].leaves_names[i];

				pi.leaves[n] = tree[n];
				pi.leaves_weight += tree[n].weight + tree[n].leaves_weight;
				tree.erase(n);
			}
			tree[pi.name] = pi;
			branches.erase(branches.begin() + idx);
		} else {
			idx++;
		}
	}

	return tree;
}

int GetRequiredBallancedWeight(const std::map<std::string, ProgInfo> tree) {
	int max = 0, ballanced = 0, val, max_weight = 0;
	//std::map<std::string, ProgInfo>::iterator it;

	for (auto it = tree.begin(); it != tree.end(); it++) {
		val = it->second.weight + it->second.leaves_weight;
		if (it == tree.begin()){
			ballanced = val;
			max = val;
			max_weight = it->second.weight;
		} else {
			if (val > max) {
				max = val;
				max_weight = it->second.weight;
			}
			if (val < max) {
				ballanced = val;
			}
		}
	}

	return max_weight - max + ballanced;
}

int main(void) {
	int result2 = 0;
	std::ifstream input;
	std::string line, result1;
	std::vector<std::string> programs;
	std::map<std::string, ProgInfo> tree;

	std::cout << "=== Advent of Code 2017 - day 7 ====" << std::endl;
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

	result1.clear();
	programs.clear();
	tree.clear();

	while (std::getline(input, line)) {
		programs.push_back(line);
	}

	if (input.is_open()) {
		input.close();
	}

	tree = BuildProgramsTree(programs);

	if (tree.size() == 1) {
		result1 = tree.begin()->second.name;
		result2 = GetRequiredBallancedWeight(tree.begin()->second.leaves);
	} else {
		result1 = "Something has failed";
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
