#include <iostream>
#include <vector>

#define TEST 0

#if TEST
#define STEPS 3
#else
#define STEPS 348
#endif

int TraceSpinLockPart1() {
	std::vector<int> buffer;
	int counter = 1;
	int current = 0, next = 0;

	buffer.clear();
	buffer.push_back(0);

	while (counter <= 2017) {
		next = ((current + STEPS) % buffer.size()) + 1;
		buffer.insert(buffer.begin() + next, counter);
		counter++;
		current = next;
	}

	return buffer[(current + 1) % buffer.size()];
}

long long TraceSpinLockPart2() {
	long long counter = 1, current = 0, next = 0, result = 0;

	while (counter <= 50000000) {
		next = ((current + STEPS) % (counter)) + 1;

		if (next == 1) {
			result = counter;
		}

		counter++;
		current = next;
	}

	return result;
}

int main(void) {
	int result1 = 0;
	long long result2 = 0;

	std::cout << "=== Advent of Code 2017 - day 17 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = TraceSpinLockPart1();
	result2 = TraceSpinLockPart2();

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
