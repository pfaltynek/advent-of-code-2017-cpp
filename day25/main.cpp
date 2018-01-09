#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

std::regex header_template("^Begin in state ([A-Z]).Perform a diagnostic checksum after (\\d+) steps.$");
std::regex state_template("^In state ([A-Z]):  If the current value is 0:    - Write the value (0|1).    - Move one slot to the (right|left).    - "
						  "Continue with state ([A-Z]).  If the current value is 1:    - Write the value (0|1).    - Move one slot to the "
						  "(left|right).    - Continue with state ([A-Z]).$");

class state {
  public:
	bool Val0SlotToLeft, Val1SlotToLeft, Val0Write0, Val1Write0;
	char Val0NextState, Val1NextState, State;
};

bool ParseHeader(std::string block, char &start_state, unsigned int &steps) {
	std::smatch sm;

	if (std::regex_match(block, sm, header_template)) {
		start_state = sm.str(1)[0];
		steps = stoi(sm.str(2));
		return true;
	} else {
		return false;
	}
}

bool ParseState(std::string block, state &st) {
	std::smatch sm;

	if (std::regex_match(block, sm, state_template)) {
		st.State = sm.str(1)[0];

		st.Val0Write0 = (sm.str(2).compare("0") == 0);
		st.Val0SlotToLeft = (sm.str(3).compare("left") == 0);
		st.Val0NextState = sm.str(4)[0];

		st.Val1Write0 = (sm.str(5).compare("0") == 0);
		st.Val1SlotToLeft = (sm.str(6).compare("left") == 0);
		st.Val1NextState = sm.str(7)[0];
		return true;
	} else {
		return false;
	}
}

unsigned int GetTuringDiagnosticChecksum(std::map<char, state> rules, char start_state, unsigned int steps) {
	int cursor = 0;
	char curr_state = start_state;
	std::map<int, bool> tape;

	tape.clear();

	for (unsigned int i = 0; i < steps; ++i) {
		int move = -1; //move left

		if (tape.find(cursor) == tape.end()) {
			// current value 0
			if (!rules[curr_state].Val0SlotToLeft) {
				move = 1;
			}
			if (!rules[curr_state].Val0Write0) {
				tape[cursor] = true;
			}
			curr_state = rules[curr_state].Val0NextState;
		} else { 
			// current value 1
			if (!rules[curr_state].Val1SlotToLeft) {
				move = 1;
			}
			if (rules[curr_state].Val1Write0) {
				tape.erase(cursor);
			}
			curr_state = rules[curr_state].Val1NextState;
		}

		cursor += move;
	}

	return tape.size();
}

int main(void) {
	unsigned int result1 = 0, result2 = 0, steps = 0, cnt = 0;
	std::ifstream input;
	std::string line, block;
	char start_state;
	std::map<char, state> states;

	std::cout << "=== Advent of Code 2017 - day 25 ====" << std::endl;
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

	block.clear();
	states.clear();

	while (std::getline(input, line) || !block.empty()) {
		cnt++;
		if (line.empty()) {
			if (!ParseHeader(block, start_state, steps)) {
				state st;
				if (!ParseState(block, st)) {
					std::cout << "Unable to decode rules at line " << cnt << std::endl;
				} else {
					states[st.State] = st;
				}
			}
			block.clear();
		} else {
			block.append(line);
		}
	}

	for (auto it = states.begin(); it != states.end(); it++) {
		if (states.find(it->second.Val0NextState) == states.end()) {
			std::cout << "Unknown next state for value 0 in state " << it->first << std::endl;
		}
		if (states.find(it->second.Val1NextState) == states.end()) {
			std::cout << "Unknown next state for value 1 in state " << it->first << std::endl;
		}
	}
	if (states.find(start_state) == states.end()) {
		std::cout << "Unknown start state" << std::endl;
	}

	if (input.is_open()) {
		input.close();
	}

	result1 = GetTuringDiagnosticChecksum(states, start_state, steps);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
