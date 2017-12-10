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

std::regex line_template("^([a-z]+) (inc|dec) (-?\\d+) if ([a-z]+) (<|>|>=|<=|==|!=) (-?\\d+)$");

typedef enum {
	EQ,  //==
	LEQ, //<=
	GEQ, //>=
	NEQ, //!=
	L,   //<
	G	//>
} CompOper;

class InstLine {
  public:
	std::string reg_name;
	bool increase;
	int inc_value;
	std::string cond_reg;
	CompOper comp_oper;
	int cond_value;
};

void TraceInstructions(std::vector<InstLine> insts, std::map<std::string, int> regs, int &result1, int &result2) {
	std::map<std::string, int>::iterator it;

	result2 = result1 = 0;

	for (unsigned int i = 0; i < insts.size(); i++) {
		bool comparison = false;
		it = regs.find(insts[i].cond_reg);
		if (it == regs.end()) {
			regs[insts[i].cond_reg] = 0;
		}
		it = regs.find(insts[i].reg_name);
		if (it == regs.end()) {
			regs[insts[i].reg_name] = 0;
		}

		switch (insts[i].comp_oper) {
			case CompOper::EQ:
				if (regs[insts[i].cond_reg] == insts[i].cond_value) {
					comparison = true;
				}
				break;
			case CompOper::NEQ:
				if (regs[insts[i].cond_reg] != insts[i].cond_value) {
					comparison = true;
				}
				break;
			case CompOper::LEQ:
				if (regs[insts[i].cond_reg] <= insts[i].cond_value) {
					comparison = true;
				}
				break;
			case CompOper::GEQ:
				if (regs[insts[i].cond_reg] >= insts[i].cond_value) {
					comparison = true;
				}
				break;
			case CompOper::G:
				if (regs[insts[i].cond_reg] > insts[i].cond_value) {
					comparison = true;
				}
				break;
			case CompOper::L:
				if (regs[insts[i].cond_reg] < insts[i].cond_value) {
					comparison = true;
				}
				break;
		}

		if (comparison) {
			if (insts[i].increase) {
				regs[insts[i].reg_name] += insts[i].inc_value;
			} else {
				regs[insts[i].reg_name] -= insts[i].inc_value;
			}

			if (regs[insts[i].reg_name] > result2) {
				result2 = regs[insts[i].reg_name];
			}
		}
	}

	for (it = regs.begin(); it != regs.end(); it++) {
		if (it->second > result1) {
			result1 = it->second;
		}
	}
}

bool DecodeInstructionLine(std::string line, InstLine &inst) {
	std::smatch sm;

	if (regex_match(line, sm, line_template)) {
		inst.reg_name = sm.str(1);
		inst.inc_value = stoi(sm.str(3));
		inst.increase = sm.str(2).compare("inc") == 0;
		inst.cond_reg = sm.str(4);
		inst.cond_value = stoi(sm.str(6));

		if (sm.str(5).compare(">") == 0) {
			inst.comp_oper = CompOper::G;
		} else if (sm.str(5).compare("<") == 0) {
			inst.comp_oper = CompOper::L;
		} else if (sm.str(5).compare(">=") == 0) {
			inst.comp_oper = CompOper::GEQ;
		} else if (sm.str(5).compare("<=") == 0) {
			inst.comp_oper = CompOper::LEQ;
		} else if (sm.str(5).compare("==") == 0) {
			inst.comp_oper = CompOper::EQ;
		} else if (sm.str(5).compare("!=") == 0) {
			inst.comp_oper = CompOper::NEQ;
		} else {
			return false;
		}
		return true;
	}
	return false;
}

int main(void) {
	int result1 = 0, result2 = 0, cnt = 0;
	std::ifstream input;
	std::string line;
	std::vector<InstLine> instructions;
	std::map<std::string, int> registers;
	InstLine inst_line;

	std::cout << "=== Advent of Code 2017 - day 8 ====" << std::endl;
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

	instructions.clear();
	registers.clear();

	while (std::getline(input, line)) {
		cnt++;
		if (DecodeInstructionLine(line, inst_line)) {
			instructions.push_back(inst_line);
		} else {
			std::cout << "Unable decode instruction at line " << cnt << std::endl;
		}
	}

	if (input.is_open()) {
		input.close();
	}

	TraceInstructions(instructions, registers, result1, result2);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
