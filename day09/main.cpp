#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define TEST 1

void AnalyzeStream(std::string stream, int &groups_score, int &garbage_chars, int &groups) {
	bool ignore = false, garbage = false;
	int group_deep;

	groups_score = groups = group_deep = garbage_chars = 0;

	for (unsigned int i = 0; i < stream.size(); i++) {
		if (ignore) {
			ignore = false;
			continue;
		}

		switch (stream[i]) {
			case '<':
				if (!garbage) {
					garbage = true;
				} else {
					garbage_chars++;
				}
				break;
			case '>':
				if (garbage) {
					garbage = false;
				}
				break;
			case '{':
				if (!garbage) {
					group_deep++;
				} else {
					garbage_chars++;
				}
				break;
			case '}':
				if (!garbage) {
					if (group_deep > 0) {
						groups++;
						groups_score += group_deep;
						group_deep--;
					} else {
						// shit
						int x = 11;
					}
				} else {
					garbage_chars++;
				}
				break;
			case '!':
				ignore = true;
				break;
			default:
				if (garbage) {
					garbage_chars++;
				}
				break;
		}
	}
}

int main(void) {
	int result1 = 0, result2 = 0, temp;
	std::ifstream input;
	std::string line, stream;

	std::cout << "=== Advent of Code 2017 - day 9 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	stream.clear();

	while (std::getline(input, line)) {
		stream.append(line);
	}

	if (input.is_open()) {
		input.close();
	}

#if TEST
	AnalyzeStream("{}", result1, result2, temp);							// 1 group with score 1
	AnalyzeStream("{{{}}}", result1, result2, temp);						// 3 groups with score 6
	AnalyzeStream("{{},{}}", result1, result2, temp);						// 3 groups with score 5
	AnalyzeStream("{{{},{},{{}}}}", result1, result2, temp);				// 6 groups with score 16
	AnalyzeStream("{<{},{},{{}}>}", result1, result2, temp);				// 1 group with score 1
	AnalyzeStream("{<a>,<a>,<a>,<a>}", result1, result2, temp);				// 1 group with score 1
	AnalyzeStream("{{<a>},{<a>},{<a>},{<a>}}", result1, result2, temp);		// 5 groups with score 9
	AnalyzeStream("{{<!>},{<!>},{<!>},{<a>}}", result1, result2, temp);		// 2 groups with score 3
	AnalyzeStream("{{<ab>},{<ab>},{<ab>},{<ab>}}", result1, result2, temp); // 5 groups with score 9
	AnalyzeStream("{{<!!>},{<!!>},{<!!>},{<!!>}}", result1, result2, temp); // 5 groups with score 9
	AnalyzeStream("{{<a!>},{<a!>},{<a!>},{<ab>}}", result1, result2, temp); // 2 groups with score 3

	AnalyzeStream("<>", result1, result2, temp);				  // 0 characters.
	AnalyzeStream("<random characters>", result1, result2, temp); // 17 characters.
	AnalyzeStream("<<<<>", result1, result2, temp);				  // 3 characters.
	AnalyzeStream("<{!>}>", result1, result2, temp);			  // 2 characters.
	AnalyzeStream("<!!>", result1, result2, temp);				  // 0 characters.
	AnalyzeStream("<!!!>>", result1, result2, temp);			  // 0 characters.
	AnalyzeStream("<{o\"i!a,<{i<a>", result1, result2, temp);	 // 10 characters.
#endif

	AnalyzeStream(stream, result1, result2, temp);
	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
