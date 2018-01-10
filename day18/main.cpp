#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <vector>

#define TEST 0

std::regex snd_template("^snd ([a-z])$");
std::regex set_template1("^set ([a-z]) (-?\\d+)$");
std::regex set_template2("^set ([a-z]) ([a-z])$");
std::regex add_template1("^add ([a-z]) (-?\\d+)$");
std::regex add_template2("^add ([a-z]) ([a-z])$");
std::regex mul_template1("^mul ([a-z]) (-?\\d+)$");
std::regex mul_template2("^mul ([a-z]) ([a-z])$");
std::regex mod_template1("^mod ([a-z]) (-?\\d+)$");
std::regex mod_template2("^mod ([a-z]) ([a-z])$");
std::regex rcv_template("^rcv ([a-z])$");
std::regex jgz_template1("^jgz ([a-z]) (-?\\d+)$");
std::regex jgz_template2("^jgz ([a-z]) ([a-z])$");
std::regex jgz_template3("^jgz (-?\\d+) (-?\\d+)$");

typedef enum { SND, SET, ADD, MUL, MOD, RCV, JGZ } InstType;

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
	std::vector<long long> queue;
	long long data_sent;
	bool sent_data_valid;
	bool waiting_data;
	long long result;
} Context;

int TraceInstruction(bool is_part1, Context &ctx) {
	bool finished = false;

	ctx.waiting_data = false;
	ctx.sent_data_valid = false;

	switch (ctx.instructions[ctx.instruction_idx].inst_type) {
		case InstType::SND:
			if (is_part1) {
				ctx.result = ctx.registers[ctx.instructions[ctx.instruction_idx].reg1];
				ctx.instruction_idx++;
			} else {
				ctx.sent_data_valid = true;
				ctx.data_sent = ctx.registers[ctx.instructions[ctx.instruction_idx].reg1];
				ctx.result++;
				ctx.instruction_idx++;
			}
			break;
		case InstType::SET:
			if (ctx.instructions[ctx.instruction_idx].arg2value) {
				ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] = ctx.instructions[ctx.instruction_idx].val2;
			} else {
				ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] = ctx.registers[ctx.instructions[ctx.instruction_idx].reg2];
			}
			ctx.instruction_idx++;
			break;
		case InstType::ADD:
			if (ctx.instructions[ctx.instruction_idx].arg2value) {
				ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] += ctx.instructions[ctx.instruction_idx].val2;
			} else {
				ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] += ctx.registers[ctx.instructions[ctx.instruction_idx].reg2];
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
			break;
		case InstType::MOD:
			if (ctx.instructions[ctx.instruction_idx].arg2value) {
				ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] %= ctx.instructions[ctx.instruction_idx].val2;
			} else {
				ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] %= ctx.registers[ctx.instructions[ctx.instruction_idx].reg2];
			}
			ctx.instruction_idx++;
			break;
		case InstType::RCV:
			if (is_part1) {
				if (ctx.registers[ctx.instructions[ctx.instruction_idx].reg1]) {
					finished = true;
				} else {
					ctx.instruction_idx++;
				}
			} else {
				if (ctx.queue.size()) {
					ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] = ctx.queue.back();
					ctx.queue.pop_back();
					ctx.instruction_idx++;
				} else {
					ctx.waiting_data = true;
				}
			}
			break;
		case InstType::JGZ:
			if (ctx.instructions[ctx.instruction_idx].arg1value) {
				if (ctx.instructions[ctx.instruction_idx].val1 > 0) {
					ctx.instruction_idx += ctx.instructions[ctx.instruction_idx].val2;
				} else {
					ctx.instruction_idx++;
				}
			} else {
				if (ctx.registers[ctx.instructions[ctx.instruction_idx].reg1] > 0) {
					if (ctx.instructions[ctx.instruction_idx].arg2value) {
						ctx.instruction_idx += ctx.instructions[ctx.instruction_idx].val2;
					} else {
						ctx.instruction_idx += ctx.registers[ctx.instructions[ctx.instruction_idx].reg2];
					}
				} else {
					ctx.instruction_idx++;
				}
			}
			break;
	}

	return finished;
}

bool ParseInstruction(std::string line, std::vector<Instruction> &instructions, std::map<char, long long> &registers) {
	std::smatch sm;
	Instruction ins;

	if (std::regex_match(line, sm, snd_template)) {
		ins.inst_type = InstType::SND;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
	} else if (std::regex_match(line, sm, set_template1)) {
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
	} else if (std::regex_match(line, sm, add_template1)) {
		ins.inst_type = InstType::ADD;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
		ins.val2 = stoi(sm.str(2));
	} else if (std::regex_match(line, sm, add_template2)) {
		ins.inst_type = InstType::ADD;
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
	} else if (std::regex_match(line, sm, mod_template1)) {
		ins.inst_type = InstType::MOD;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
		ins.val2 = stoi(sm.str(2));
	} else if (std::regex_match(line, sm, mod_template2)) {
		ins.inst_type = InstType::MOD;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
		ins.arg2value = false;
		ins.reg2 = sm.str(2)[0];
	} else if (std::regex_match(line, sm, rcv_template)) {
		ins.inst_type = InstType::RCV;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
	} else if (std::regex_match(line, sm, jgz_template1)) {
		ins.inst_type = InstType::JGZ;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
		ins.val2 = stoi(sm.str(2));
	} else if (std::regex_match(line, sm, jgz_template2)) {
		ins.inst_type = InstType::JGZ;
		ins.arg1value = false;
		ins.reg1 = sm.str(1)[0];
		ins.arg2value = false;
		ins.reg2 = sm.str(2)[0];
	} else if (std::regex_match(line, sm, jgz_template3)) {
		ins.inst_type = InstType::JGZ;
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

int main(void) {
	int cnt = 0;
	std::ifstream input;
	std::string line;
	std::vector<Instruction> instructions;
	std::map<char, long long> registers;
	Context ctx0, ctx1;
	int deadlocks;

	std::cout << "=== Advent of Code 2017 - day 18 ====" << std::endl;
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

	ctx0.instruction_idx = 0;
	ctx0.instructions = instructions;
	ctx0.registers = registers;
	ctx0.data_sent = 0;
	ctx0.sent_data_valid = false;
	ctx0.waiting_data = false;
	ctx0.result = 0;
	ctx0.queue.clear();

	ctx1 = ctx0;

	while (!TraceInstruction(true, ctx0)) {
	}

	ctx0 = ctx1;
	ctx0.registers['p'] = 0;
	ctx1.registers['p'] = 1;

	deadlocks = 0;

	do {
		TraceInstruction(false, ctx0);
		if (ctx0.sent_data_valid) {
			ctx1.queue.insert(ctx1.queue.begin(), ctx0.data_sent);
		}

		TraceInstruction(false, ctx1);
		if (ctx1.sent_data_valid) {
			ctx0.queue.insert(ctx0.queue.begin(), ctx1.data_sent);
		}

		if (ctx0.waiting_data && ctx1.waiting_data){
			deadlocks++;
		} else {
			deadlocks = 0;
		}

	} while (deadlocks < 3);

	std::cout << "Result is " << ctx0.result << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << ctx1.result << std::endl;
}
