#include <cstdio>
#include <cstring>
#include <cstdint>

#define S2_IMPL
#include "s2/s2list.h"

struct Pattern
{
	bool plants[5] = { false };
	bool result = false;
};

int main()
{
	// Load initial state into memory with a negative offset
	s2::list<bool> plants;

	int offset = 500;
	for (int i = 0; i < offset; i++) {
		plants.add(false);
	}

	FILE* fh = fopen("input/12.input", "r");
	fseek(fh, 15, SEEK_CUR);

	while (true) {
		char c = fgetc(fh);

		if (c == '.') {
			plants.add(false);
		} else if (c == '#') {
			plants.add(true);
		} else if (c == '\n') {
			break;
		}
	}

	// Add a positive offset too
	for (int i = 0; i < offset; i++) {
		plants.add(false);
	}

	// Load patterns into memory
	s2::list<Pattern> patterns;

	fgetc(fh);
	char buffer[14];
	while (fgets(buffer, 14, fh)) {
		size_t len = strlen(buffer);
		buffer[--len] == '\0';
		if (len == 0) {
			break;
		}

		char ll, l, c, r, rr, res;
		sscanf(buffer, "%c%c%c%c%c => %c", &ll, &l, &c, &r, &rr, &res);

		auto &newPattern = patterns.add();
		newPattern.plants[0] = (ll == '#');
		newPattern.plants[1] = (l == '#');
		newPattern.plants[2] = (c == '#');
		newPattern.plants[3] = (r == '#');
		newPattern.plants[4] = (rr == '#');
		newPattern.result = (res == '#');
	}
	fclose(fh);

	// Run the generations
	int last_diff = 0;
	int last_sum = 0;

	for (int i = 0; i < offset; i++) {
		auto plantsCopy = plants;

		int plant_sum = 0;

		for (size_t j = 0; j < plants.len(); j++) {
			for (auto &pattern : patterns) {
				bool foundPattern = true;
				for (int k = 0; k < 5; k++) {
					if (pattern.plants[k] != plantsCopy[j + k]) {
						foundPattern = false;
						break;
					}
				}
				if (!foundPattern) {
					continue;
				}

				int index = j + 2;
				plants[index] = pattern.result;

				if (pattern.result) {
					plant_sum += index - offset;
				}
			}
		}

		// Part 1: Find the sum of all plants after 20 generations
		if (i == 19) {
			printf("Part 1: %d\n", plant_sum);
		}

		// Part 2: Find the sum of all plants after 50000000000 generations
		int diff = plant_sum - last_sum;
		if (diff == last_diff) {
			uint64_t part2 = plant_sum + (50000000000 - i - 1) * diff;
			printf("Part 2: %lu\n", part2);
			break;
		}

		last_sum = plant_sum;
		last_diff = diff;
	}

	return 0;
}
