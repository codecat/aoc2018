#include <cstdio>
#include <cstring>

#define S2_IMPL
#include "s2/s2list.h"

int main()
{
	// Load input from file
	int numberInput = 0;

	FILE* fh = fopen("input/14.input", "r");
	fscanf(fh, "%d", &numberInput);
	fclose(fh);

	// Make initial states
	int current[2] = { 0, 1 };
	s2::list<char> seen;

	seen.add(3);
	seen.add(7);

	bool didPart1 = false;
	bool didPart2 = false;
	int lastPart2 = 4;

	// Make a buffer of the numbers to expect in sequence
	char numberInputString[12] = { 0 };
	sprintf(numberInputString, "%d", numberInput);
	size_t lenInputString = strlen(numberInputString);
	for (size_t i = 0; i < lenInputString; i++) {
		numberInputString[i] -= 48;
	}

	// Walk through all steps
	while (!didPart1 || !didPart2) {
		char a = seen[current[0]];
		char b = seen[current[1]];

		char c = (a + b) / 10;
		if (c > 0) {
			seen.add(c);
		}

		char d = (a + b) % 10;
		seen.add(d);

		current[0] = (current[0] + 1 + a) % (int)seen.len();
		current[1] = (current[1] + 1 + b) % (int)seen.len();

		// Part 1: Find the scores of ten recipes immediately after the number of recipes in the input
		if (!didPart1 && (int)seen.len() >= numberInput + 10) {
			printf("Part 1: ");
			for (int i = numberInput; i < numberInput + 10; i++) {
				printf("%d", seen[i]);
			}
			putchar('\n');
			didPart1 = true;
		}

		// Part 2: Find the number of recipes on the left of the input sequence
		if (!didPart2 && (int)seen.len() - lastPart2 >= 100) {
			for (int i = lastPart2 - 4; i < (int)seen.len(); i++) {
				if (memcmp(&seen[i], numberInputString, lenInputString) == 0) {
					printf("Part 2: %d\n", i);
					didPart2 = true;
					break;
				}
			}
			lastPart2 = (int)seen.len();
		}
	}

	return 0;
}
