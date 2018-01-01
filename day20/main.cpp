#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define TEST 0

std::regex particle_template("p=<(-?\\d+),(-?\\d+),(-?\\d+)>, v=<(-?\\d+),(-?\\d+),(-?\\d+)>, a=<(-?\\d+),(-?\\d+),(-?\\d+)>");

typedef struct { long long x, y, z; } dim3;

typedef struct { dim3 particle, velocity, acceleration; } particle;

bool ParseParticle(std::string line, particle &part) {
	std::smatch sm;

	if (std::regex_match(line, sm, particle_template)) {
		part.particle.x = std::stoi(sm.str(1));
		part.particle.y = std::stoi(sm.str(2));
		part.particle.z = std::stoi(sm.str(3));
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

long long GetParticleDistance(particle part, int step) {
	dim3 result = part.particle;

	result.x += part.velocity.x;
	result.y += part.velocity.y;
	result.z += part.velocity.z;

	result.x += step * part.acceleration.x;
	result.y += step * part.acceleration.y;
	result.z += step * part.acceleration.z;

	return abs(result.x) + abs(result.y) + abs(result.z);
}

int GetClosestParticle(std::vector<particle> particles, int step) {
	long long closest = __LONG_LONG_MAX__, tmp;
	unsigned int result = 0;

	for (unsigned int i = 0; i < particles.size(); ++i) {
		tmp = GetParticleDistance(particles[i], step);
		if (tmp < closest) {
			result = i;
			closest = tmp;
		}
	}

	return result;
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
/*	result1 = GetClosestParticle(particles, 20000);
	result1 = GetClosestParticle(particles, 30000);
	result1 = GetClosestParticle(particles, 40000);
	result1 = GetClosestParticle(particles, 50000);
	result1 = GetClosestParticle(particles, 60000);
	result1 = GetClosestParticle(particles, 70000);
	result1 = GetClosestParticle(particles, 80000);
	result1 = GetClosestParticle(particles, 90000);
	result1 = GetClosestParticle(particles, 100000);
	result1 = GetClosestParticle(particles, 110000);
*/

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
