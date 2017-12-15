#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

std::regex line_template("^(\\d+): (\\d+)$");

class Scanner {
  public:
	int GetDepth() {
		return _depth;
	}
	int GetRange() {
		return _range;
	}
	int GetState() {
		return _state;
	}
	Scanner() {
		_depth = 0;
		_range = 1;
		_state = 0;
		_down = true;
	}
	Scanner(int depth, int range) {
		Init(depth, range);
	}
	void Init(int depth, int range) {
		_depth = depth;
		_range = range;
		_state = 0;
		_down = true;
	}
	void Reset() {
		_state = 0;
		_down = true;
	}
	void PicoSecondRunOut() {
		if (_down) {
			if (++_state == _range) {
				_state -= 2;
				_down = false;
			}
		} else {
			if (--_state == -1) {
				_state += 2;
				_down = true;
			}
		}
	}

  private:
	int _depth, _range, _state;
	bool _down;
};

bool ParseLine(std::string line, std::map<int, Scanner> &firewall) {
	std::smatch sm;
	int d, r;

	if (std::regex_match(line, sm, line_template)) {
		d = stoi(sm.str(1));
		r = stoi(sm.str(2));
		firewall[d] = Scanner(d, r);
		return true;
	}
	return false;
}

int GetDelayToPassFirewall(std::map<int, Scanner> firewall) {
	bool caught = true;
	int max = 0, delay = 0;

	for (auto it = firewall.begin(); it != firewall.end(); it++) {
		if (it->first > max) {
			max = it->first;
		}
	}

	while (caught) {
		for (auto it = firewall.begin(); it != firewall.end(); it++) {
			it->second.Reset();
		}

		for (int i = 0; i < delay; i++) {
			for (auto it = firewall.begin(); it != firewall.end(); it++) {
				it->second.PicoSecondRunOut();
			}
		}

		caught = false;

		for (int i = 0; i <= max; i++) {
			if (firewall.find(i) != firewall.end()) {
				if (firewall[i].GetState() == 0) {
					caught = true;
					break;
				}
			}
			for (auto it = firewall.begin(); it != firewall.end(); it++) {
				it->second.PicoSecondRunOut();
			}
		}
		delay++;
	}

	return --delay;
}

int GetFirewallSeverity(std::map<int, Scanner> firewall) {
	int max = 0, severity = 0;

	for (auto it = firewall.begin(); it != firewall.end(); it++) {
		if (it->first > max) {
			max = it->first;
			it->second.Reset();
		}
	}

	for (int i = 0; i <= max; i++) {
		if (firewall.find(i) != firewall.end()) {
			if (firewall[i].GetState() == 0) {
				severity += i * firewall[i].GetRange();
			}
		}

		for (auto it = firewall.begin(); it != firewall.end(); it++) {
			it->second.PicoSecondRunOut();
		}
	}

	return severity;
}

int main(void) {
	int result1 = 0, result2 = 0, cnt = 0;
	std::ifstream input;
	std::string line;
	std::map<int, Scanner> firewall;

	std::cout << "=== Advent of Code 2017 - day 13 ====" << std::endl;
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

	firewall.clear();

	while (std::getline(input, line)) {
		cnt++;
		if (!ParseLine(line, firewall)) {
			std::cout << "Invalid program info on line " << cnt << std::endl;
		}
	}

	if (input.is_open()) {
		input.close();
	}

	result1 = GetFirewallSeverity(firewall);
	result2 = GetDelayToPassFirewall(firewall);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
