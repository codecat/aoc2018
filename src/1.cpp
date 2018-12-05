#include <cstdio>
#include <cstring>

#define S2_IMPL
#include "s2/s2list.h"

int main()
{
	// Read all delta values
	s2::list<int> deltas;

	FILE* fh = fopen("input/1.input", "r");
	char buffer[24];
	while (fgets(buffer, 24, fh)) {
		size_t len = strlen(buffer);
		buffer[--len] = '\0';
		if (len == 0) {
			break;
		}

		char op;
		int delta = 0;
		if (sscanf(buffer, "%c%d", &op, &delta) == 2) {
			if (op == '-') {
				delta = -delta;
			}

			deltas.add(delta);
		}
	}

	// Part 1: Sum all deltas
	int sum = 0;
	for (int delta : deltas) {
		sum += delta;
	}
	printf("Part 1: %d\n", sum);

	// Part 2: Find doubles (TODO: Optimize)
	sum = 0;
	for (size_t i = 0; ; i++) {
		int delta = deltas[i % deltas.len()];
		sum += delta;

		bool found = false;
		int subsum = 0;
		for (size_t j = 0; j < i; j++) {
			int subdelta = deltas[j % deltas.len()];
			subsum += subdelta;

			if (subsum == sum) {
				printf("Part 2: %d\n", subsum);
				found = true;
				break;
			}
		}

		if (found) {
			break;
		}
	}

	fclose(fh);
	return 0;
}
