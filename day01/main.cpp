#include <fstream>
#include <iostream>

int SolveCaptcha1stPart(std::string captcha) {
	int result = 0;

	for (int i = 1; i < captcha.size(); i++) {
		if (captcha[i - 1] == captcha[i]) {
			result += captcha[i] - '0';
		}
	}
	if (captcha[captcha.size() - 1] == captcha[0]) {
		result += captcha[0] - '0';
	}

	return result;
}

int SolveCaptcha2ndPart(std::string captcha) {
	int result = 0;
	int shift = 0;

	for (int i = 0; i < captcha.size(); i++) {
		shift = (i + (captcha.size() / 2)) % captcha.size();
		if (captcha[i] == captcha[shift]) {
			result += captcha[i] - '0';
		}
	}

	return result;
}

int main(void) {
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::string captcha;

	std::cout << "=== Advent of Code 2017 - day 1 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	captcha.clear();
	while (std::getline(input, line)) {
		captcha += line;
	}

	if (input.is_open()) {
		input.close();
	}

#if TEST
	result1 = SolveCaptcha1stPart("1122");
	result1 = SolveCaptcha1stPart("1111");
	result1 = SolveCaptcha1stPart("1234");
	result1 = SolveCaptcha1stPart("91212129");

	result2 = SolveCaptcha2ndPart("1212");
	result2 = SolveCaptcha2ndPart("1221");
	result2 = SolveCaptcha2ndPart("123425");
	result2 = SolveCaptcha2ndPart("123123");
	result2 = SolveCaptcha2ndPart("12131415");
#endif

	result1 = SolveCaptcha1stPart(captcha);
	result2 = SolveCaptcha2ndPart(captcha);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
