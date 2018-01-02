#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

std::regex particle_template("p=<(-?\\d+),(-?\\d+),(-?\\d+)>, v=<(-?\\d+),(-?\\d+),(-?\\d+)>, a=<(-?\\d+),(-?\\d+),(-?\\d+)>");

typedef struct {
	long long x, y, z;
} dim3;

typedef struct {
	dim3 position, velocity, acceleration;
} particle;

bool ParseParticle(std::string line, particle &part) {
	std::smatch sm;

	if (std::regex_match(line, sm, particle_template)) {
		part.position.x = std::stoi(sm.str(1));
		part.position.y = std::stoi(sm.str(2));
		part.position.z = std::stoi(sm.str(3));
		part.velocity.x = std::stoi(sm.str(4));
		part.velocity.y = std::stoi(sm.str(5));
		part.velocity.z = std::stoi(sm.str(6));
		part.acceleration.x = std::stoi(sm.str(7));
		part.acceleration.y = std::stoi(sm.str(8));
		part.acceleration.z = std::stoi(sm.str(9));
		return true;
	}

	return false;
}

unsigned long long GetParticleDistance(particle part) {
	return ((unsigned long long)abs(part.position.x) + (unsigned long long)abs(part.position.y) + (unsigned long long)abs(part.position.z));
}

void UpdateParticle(particle &part) {
	part.velocity.x += part.acceleration.x;
	part.velocity.y += part.acceleration.y;
	part.velocity.z += part.acceleration.z;

	part.position.x += part.velocity.x;
	part.position.y += part.velocity.y;
	part.position.z += part.velocity.z;
}

int GetClosestParticle(std::vector<particle> particles, int step) {
	unsigned long long closest = __LONG_LONG_MAX__, tmp;
	int idx = -1, result = -1;

	for (int j = 0; j < step; ++j) {
		closest = __LONG_LONG_MAX__;
		idx = -1;
		for (unsigned int i = 0; i < particles.size(); ++i) {
			UpdateParticle(particles[i]);
			tmp = GetParticleDistance(particles[i]);
			if (tmp < closest) {
				idx = i;
				closest = tmp;
			}
		}
		if (idx != result) {
			result = idx;
		}
	}

	return result;
}

int ClearCollisions(std::vector<particle> particles) {
	int cnt = 0;
	std::map<int, particle> map;
	std::vector<int> remove;
	bool collided;
	map.clear();
	unsigned int part_size;

	for (unsigned int i = 0; i < particles.size(); ++i) {
		map[i] = particles[i];
	}

	part_size = map.size();

	while (cnt < 10) {
		remove.clear();

		for (auto it = map.begin(); it != map.end(); it++) {
			UpdateParticle(it->second);
		}

		for (auto it = map.begin(); it != map.end(); it++) {
			std::map<int, particle>::iterator it2 = it;
			it2++;
			collided = false;
			while (it2 != map.end()) {
				if ((it->second.position.x == it2->second.position.x) && (it->second.position.y == it2->second.position.y) &&
					(it->second.position.z == it2->second.position.z)) {
					collided = true;
					remove.push_back(it2->first);
				}
				it2++;
			}
			if (collided) {
				remove.push_back(it->first);
			}
		}

		for (unsigned int i = 0; i < remove.size(); ++i) {
			map.erase(remove[i]);
		}

		if (part_size == map.size()) {
			cnt++;
		} else {
			cnt = 0;
			part_size = map.size();
		}
	}

	return map.size();
}

int main(void) {
	int cnt = 0, result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::vector<particle> particles;
	std::cout << "=== Advent of Code 2017 - day 20 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;
	particle part;

#if TEST
	input.open("input-test.txt", std::ifstream::in);
#else
	input.open("input.txt", std::ifstream::in);
#endif

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	particles.clear();

	while (std::getline(input, line)) {
		if (ParseParticle(line, part)) {
			particles.push_back(part);
		} else {
			std::cout << "Unable to decode particle " << cnt << std::endl;
		}
		cnt++;
	}

	if (input.is_open()) {
		input.close();
	}

	result1 = GetClosestParticle(particles, 10000);
	result2 = ClearCollisions(particles);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
