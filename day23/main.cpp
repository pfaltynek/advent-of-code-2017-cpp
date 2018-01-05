#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

std::regex set_template1("^set ([a-z]) (-?\\d+)$");
std::regex set_template2("^set ([a-z]) ([a-z])$");
std::regex sub_template1("^sub ([a-z]) (-?\\d+)$");
std::regex sub_template2("^sub ([a-z]) ([a-z])$");
std::regex mul_template1("^mul ([a-z]) (-?\\d+)$");
std::regex mul_template2("^mul ([a-z]) ([a-z])$");
std::regex jnz_template1("^jnz ([a-z]) (-?\\d+)$");
std::regex jnz_template2("^jnz (-?\\d+) (-?\\d+)$");

typedef enum { SET, SUB, MUL, JNZ } InstType;

class Instruction {
  public:
	InstType inst_type;
	long long val1, val2;
	char reg1, reg2;
	bool arg1value, arg2value;
	std::string text;

	Instruction() {
		val1 = val2 = 0;
		arg1value = arg2value = true;
		reg1 = reg2 = '?';
		text.clear();
	}
};

typedef struct {
	int instruction_idx;
	std::vector<Instruction> instructions;
	std::map<char, long long> registers;
	long long result;
} Context;

void TraceInstructions(Context &ctx) {

	while ((ctx.instruction_idx < (int)ctx.instructions.size()) && (ctx.instruction_idx >= 0)) {
		switch (ctx.instructions[ctx.instruction_idx].inst_type) {
			case InstType::SET:
				if (ctx.instructions[ctx.instruction_idx].arg2value) {
					ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] = ctx.instructions[ctx.instruction_idx].val2;
				} else {
					ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] = ctx.registers[ctx.instructions[ctx.instruction_idx].reg2];
				}
				ctx.instruction_idx++;
				break;
			case InstType::SUB:
				if (ctx.instructions[ctx.instruction_idx].arg2value) {
					ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] -= ctx.instructions[ctx.instruction_idx].val2;
				} else {
					ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] -= ctx.registers[ctx.instructions[ctx.instruction_idx].reg2];
				}
				ctx.instruction_idx++;
				break;
			case InstType::MUL:
				if (ctx.instructions[ctx.instruction_idx].arg2value) {
					ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] *= ctx.instructions[ctx.instruction_idx].val2;
				} else {
					ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] *= ctx.registers[ctx.instructions[ctx.instruction_idx].reg2];
				}
				ctx.instruction_idx++;
				ctx.result++;
				break;
			case InstType::JNZ:
				if (ctx.instructions[ctx.instruction_idx].arg1value) {
					if (ctx.instructions[ctx.instruction_idx].val1 != 0) {
						ctx.instruction_idx += ctx.instructions[ctx.instruction_idx].val2;
					} else {
						ctx.instruction_idx++;
					}
				} else {
					if (ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] != 0) {
						ctx.instruction_idx += ctx.instructions[ctx.instruction_idx].val2;
					} else {
						ctx.instruction_idx++;
					}
				}
				break;
		}
	}
}

bool ParseInstruction(std::string line, std::vector<Instruction> &instructions, std::map<char, long long> &registers) {
	std::smatch sm;
	Instruction ins;

	if (std::regex_match(line, sm, set_template1)) {
		ins.inst_type = InstType::SET;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
		ins.val2 = stoi(sm.str(2));
	} else if (std::regex_match(line, sm, set_template2)) {
		ins.inst_type = InstType::SET;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
		ins.arg2value = false;
		ins.reg2 = sm.str(2)[0];
	} else if (std::regex_match(line, sm, sub_template1)) {
		ins.inst_type = InstType::SUB;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
		ins.val2 = stoi(sm.str(2));
	} else if (std::regex_match(line, sm, sub_template2)) {
		ins.inst_type = InstType::SUB;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
		ins.arg2value = false;
		ins.reg2 = sm.str(2)[0];
	} else if (std::regex_match(line, sm, mul_template1)) {
		ins.inst_type = InstType::MUL;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
		ins.val2 = stoi(sm.str(2));
	} else if (std::regex_match(line, sm, mul_template2)) {
		ins.inst_type = InstType::MUL;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
		ins.arg2value = false;
		ins.reg2 = sm.str(2)[0];
	} else if (std::regex_match(line, sm, jnz_template1)) {
		ins.inst_type = InstType::JNZ;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
		ins.val2 = stoi(sm.str(2));
	} else if (std::regex_match(line, sm, jnz_template2)) {
		ins.inst_type = InstType::JNZ;
		ins.val1 = stoi(sm.str(1));
		ins.val2 = stoi(sm.str(2));
	} else {
		return false;
	}

	if (!ins.arg1value) {
		registers[ins.reg1] = 0;
	}
	if (!ins.arg2value) {
		registers[ins.reg2] = 0;
	}
	ins.text = line;
	instructions.push_back(ins);

	return true;
}

int Part2OptimizedAndRewritten(){
	int cnt = 0;

	for (int i = 109900; i <= 126900; i+=17) {
		for (int j = 2; j <= i / 2; j++) {
			if ((i % j) == 0){
				cnt++;
				break;
			}
		}
	}

	return cnt;
}

int main(void) {
	int cnt = 0;
	std::ifstream input;
	std::string line;
	std::vector<Instruction> instructions;
	std::map<char, long long> registers;
	Context ctx;

	std::cout << "=== Advent of Code 2017 - day 23 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	registers.clear();
	instructions.clear();

	while (std::getline(input, line)) {
		cnt++;
		if (!ParseInstruction(line, instructions, registers)) {
			input.close();
			std::cout << "Unknown instruction on line " << cnt << std::endl;
			return -1;
		}
	}

	if (input.is_open()) {
		input.close();
	}

	for (char ch = 'a'; ch <= 'h'; ++ch) {
		registers[ch] = 0;
	}

	ctx.instruction_idx = 0;
	ctx.instructions = instructions;
	ctx.registers = registers;
	ctx.result = 0;

	TraceInstructions(ctx);

	std::cout << "Result is " << ctx.result << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	// to solve the part 2 in reasonable time, the code listing have been analyzed (reverse-engineered) optimized and rewritten
	cnt = Part2OptimizedAndRewritten();

	std::cout << "Result is " << cnt << std::endl;
}
