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
	int weight, weight_leaves;
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
			pi.weight_leaves = 0;
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
			pi.weight_leaves = 0;
			pi.leaves.clear();

			for (unsigned int i = 0; i < branches[idx].leaves_names.size(); i++) {
				std::string n;
				n = branches[idx].leaves_names[i];

				pi.leaves[n] = tree[n];
				pi.weight_leaves += tree[n].weight + tree[n].weight_leaves;
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

void GetRequiredBallancedWeight(ProgInfo tree_root, int &result, int diff) {
	int ballanced = 0, overload = 0, sum;
	ProgInfo imbalanced;

	for (auto it = tree_root.leaves.begin(); it != tree_root.leaves.end(); it++) {
		sum = it->second.weight + it->second.weight_leaves;
		if (it == tree_root.leaves.begin()) {
			ballanced = overload = sum;
			imbalanced = it->second;
		} else {
			if (sum > ballanced){
				overload = sum;
				imbalanced = it->second;
			}
			if (sum < ballanced){
				ballanced = sum;
			}
		}
	}
	if (ballanced != overload){
		GetRequiredBallancedWeight(imbalanced, result, overload - ballanced);
	} else {
		result = tree_root.weight - diff;
	}
}

int main(void) {
	int result2 = 0, sum = 0;
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
		GetRequiredBallancedWeight(tree.begin()->second, result2, sum);
	} else {
		result1 = "Something has failed";
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
