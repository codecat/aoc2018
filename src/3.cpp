#include <cstdio>
#include <cstring>
#include <cstdint>

#define S2_IMPL
#include "s2/s2list.h"

class Rect
{
public:
	int x;
	int y;
	int w;
	int h;
};

int main()
{
	// Load all rectangles in memory
	s2::list<Rect> rects;

	FILE* fh = fopen("input/3.input", "r");
	char buffer[80];
	while (!feof(fh)) {
		fgets(buffer, 80, fh);

		size_t len = strlen(buffer);
		if (len == 0) {
			continue;
		}

		buffer[--len] = '\0';
		if (len == 0) {
			continue;
		}

		int id;
		Rect &r = rects.add();
		sscanf(buffer, "#%d @ %d,%d: %dx%d", &id, &r.x, &r.y, &r.w, &r.h);
	}
	fclose(fh);

	// Find maximum size of the input (actually 1000x1000, but input only has 999x1000)
	int size_x = 0;
	int size_y = 0;

	for (auto &r : rects) {
		int right = r.x + r.w;
		int bottom = r.y + r.h;

		if (right > size_x) {
			size_x = right;
		}

		if (bottom > size_y) {
			size_y = bottom;
		}
	}

	printf("Size: %d x %d\n", size_x, size_y);

	// Map out the fabric in a buffer
	uint16_t* fabric = (uint16_t*)malloc((size_x * size_y) * sizeof(uint16_t));
	memset(fabric, 0, (size_x * size_y) * sizeof(uint16_t));

	for (size_t i = 0; i < rects.len(); i++) {
		auto &r = rects[i];
		for (int y = r.y; y < r.y + r.h; y++) {
			int row = y * size_x;
			for (int x = r.x; x < r.x + r.w; x++) {
				uint16_t &u = fabric[row + x];
				if (u == 0) {
					u = (uint16_t)i + 1;
				} else {
					u = 0xFFFF;
				}
			}
		}
	}

	// Part 1: Get amount of overlapping units
	int overlapped = 0;
	for (int y = 0; y < size_y; y++) {
		int row = y * size_x;
		for (int x = 0; x < size_x; x++) {
			if (fabric[row + x] == 0xFFFF) {
				overlapped++;
			}
		}
	}

	printf("Part 1: %d\n", overlapped);

	// Part 2: Find untouched rect
	for (int y = 0; y < size_y; y++) {
		int row = y * size_x;
		bool found = false;
		for (int x = 0; x < size_x; x++) {
			uint16_t &u = fabric[row + x];
			if (u == 0 || u == 0xFFFF) {
				continue;
			}

			bool rectMatches = true;

			auto &r = rects[u - 1];
			for (int yy = r.y; yy < r.y + r.h; yy++) {
				int rectrow = yy * size_x;
				bool cancel = false;
				for (int xx = r.x; xx < r.x + r.w; xx++) {
					if (fabric[rectrow + xx] != u) {
						rectMatches = false;
						cancel = true;
					}
				}
				if (cancel) {
					break;
				}
			}

			if (rectMatches) {
				printf("Part 2: %d (@ %d,%d - %dx%d)\n", (int)u, r.x, r.y, r.w, r.h);
				found = true;
				break;
			}
		}

		if (found) {
			break;
		}
	}


	// Clean up
	free(fabric);

	return 0;
}
