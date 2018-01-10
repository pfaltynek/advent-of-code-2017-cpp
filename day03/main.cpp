#include <iostream>
#include <map>
#include <vector>

#define TEST 0

typedef struct POSITION {
	int x, y;
} POSITION;

const int input = 289326;

std::string GetCoord(int x, int y) {
	return std::to_string(x) + ";" + std::to_string(y);
}

void BuildSpiralMap(int input, int &part1result, unsigned int &part2result) {
	std::vector<POSITION> grid;
	std::map<std::string, unsigned int> values;
	int x, y, t, l, r, b;
	POSITION pos;
	unsigned char head = 0;

	x = y = t = l = r = b = 0;
	part2result = 0;

	grid.clear();
	pos.x = x;
	pos.y = y;
	grid.push_back(pos);
	grid.push_back(pos);

	values.clear();
	values[GetCoord(x, y)] = 1;

	while ((grid.size() <= input) || (part2result == 0)) {
		switch (head) {
			case 0:
				x++;
				if (x > r) {
					head++;
					head %= 4;
					r = x;
				}
				break;
			case 1:
				y++;
				if (y > t) {
					head++;
					head %= 4;
					t = y;
				}
				break;
			case 2:
				x--;
				if (x < l) {
					head++;
					head %= 4;
					l = x;
				}
				break;
			case 3:
				y--;
				if (y < b) {
					head++;
					head %= 4;
					b = y;
				}
				break;
			default:
				int z = 13; // problem
				break;
		}
		pos.x = x;
		pos.y = y;
		grid.push_back(pos);
		if (part2result == 0) {
			unsigned int val = 0;
			std::string coord, target_coord;

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					coord = GetCoord(x + i, y + j);
					if ((i == 0) && (j == 0)) {
						target_coord = coord;
					} else {
						if (values.find(coord) != values.end()) {
							val += values[coord];
						}
					}
				}
			}

			values[target_coord] = val;
			if (val > input) {
				part2result = val;
			}
		}
	}

	part1result = std::abs(grid[input].x) + std::abs(grid[input].y);
}

int main(void) {
	unsigned int result2 = 0;
	int result1 = 0;

	std::cout << "=== Advent of Code 2017 - day 3 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST
	BuildSpiralMap(1, result1, result2);
	BuildSpiralMap(12, result1, result2);
	BuildSpiralMap(23, result1, result2);
	BuildSpiralMap(1024, result1, result2);
#endif

	BuildSpiralMap(input, result1, result2);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
