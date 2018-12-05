#include <cstdio>
#include <cstring>

#define S2_IMPL
#include "s2/s2string.h"
#include "s2/s2list.h"

int main()
{
	// Read all the lines
	s2::list<s2::string> lines;

	FILE* fh = fopen("input/2.input", "r");
	char buffer[30];
	while (fgets(buffer, 30, fh)) {
		size_t len = strlen(buffer);
		buffer[--len] = '\0';

		if (len != 0) {
			lines.add(buffer);
		}
	}

	// Part 1: Get checksum
	int countA = 0;
	int countB = 0;

	for (auto &line : lines) {
		int counts[26];
		memset(counts, 0, sizeof(counts));

		const char* p = line;
		while (*p != '\0') {
			counts[*p - 97]++;
			p++;
		}

		bool a = false;
		bool b = false;
		for (int i = 0; i < 26; i++) {
			int num = counts[i];
			if (num == 3 && !b) {
				countB++;
				b = true;
			} else if (num == 2 && !a) {
				countA++;
				a = true;
			}
		}
	}

	int checksum = countA * countB;
	printf("Part 1: %d\n", checksum);

	// Part 2: Find single differing character
	for (auto &line : lines) {
		bool found = false;

		for (auto &subline : lines) {
			int diff = 0;
			int diffIndex = 0;

			for (size_t i = 0; i < line.len(); i++) {
				if (line[i] != subline[i]) {
					diff++;
					diffIndex = i;
				}
				if (diff > 1) {
					break;
				}
			}
			
			if (diff == 1) {
				s2::string diffLine = line.substr(0, diffIndex);
				diffLine += line.substr(diffIndex + 1);
				printf("Part 2: %s (diff char at index %d)\n", diffLine.c_str(), diffIndex);
				found = true;
				break;
			}
		}

		if (found) {
			break;
		}
	}

	return 0;
}
