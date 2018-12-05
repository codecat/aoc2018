#include <cstdio>
#include <cstring>

static int reactPolymer(char* buffer, size_t length)
{
	char* p = buffer;
	char* pn = p + 1;

	int ret = 0;

	while (p < buffer + length) {
		char &a = *p;
		char &b = *pn;

		if (a == b || (a != b - 32 && a - 32 != b)) {
			// "aa", "AA", "ab", "ba", etc.
			p = pn;
			pn++;
			continue;
		}

		// "aA", "Aa"
		a = b = '\0';
		ret += 2;

		pn++;
		if (p == buffer) {
			p = pn;
			pn++;
		} else {
			while (p > buffer && *p == '\0') {
				p--;
			}
		}
	}

	return length - ret;
}

static const size_t g_size = 50000;
static char g_buffer[g_size];

int main()
{
	// Load the input string
	FILE* fh = fopen("5.input", "r");
	fread(g_buffer, 1, g_size, fh);
	fclose(fh);

	char buffer[g_size] = { 0 };

	// Part 1: Find length of base polymer
	memcpy(buffer, g_buffer, g_size);
	int baseLength = reactPolymer(buffer, g_size);

	printf("Part 1: %d\n", baseLength);

	// Part 2: Find minimum possible length after removing each unit type
	int smallest = -1;
	for (int i = 0; i < 26; i++) {
		char cl = 'a' + i;
		char cu = 'A' + i;

		size_t pos = 0;
		for (size_t i = 0; i < g_size; i++) {
			char c = g_buffer[i];
			if (c != cl && c != cu) {
				buffer[pos++] = c;
			}
		}

		int length = reactPolymer(buffer, pos);
		if (smallest == -1 || length < smallest) {
			smallest = length;
		}
	}

	printf("Part 2: %d\n", smallest);

	return 0;
}
