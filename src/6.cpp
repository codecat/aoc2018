#include <cstdio>
#include <cstring>
#include <cmath>

#include <cstdint>
#include <cinttypes>

#define S2_IMPL
#include "s2/s2list.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

struct Point
{
	int16_t x = 0;
	int16_t y = 0;

	int area = 1;
	bool finite = true;

	Point() { }
	Point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	Point(const Point &p)
	{
		x = p.x;
		y = p.y;
	}

	int16_t distance(const Point &p)
	{
		return abs(p.x - x) + abs(p.y - y);
	}

	bool operator ==(const Point &p)
	{
		return x == p.x && y == p.y;
	}
};

int main()
{
	// Load points from file
	s2::list<Point> points;

	FILE* fh = fopen("input/6.input", "r");
	char buffer[16];
	while (fgets(buffer, 16, fh)) {
		size_t len = strlen(buffer);
		buffer[--len] = '\0';
		if (len == 0) {
			break;
		}

		auto &p = points.add();
		sscanf(buffer, "%" SCNd16 ", %" SCNd16, &p.x, &p.y);
	}
	fclose(fh);

	// Find boundaries of grid
	int top, bottom, left, right;
	top = bottom = left = right = -1;

	for (auto &p : points) {
		if (top == -1 || p.y < top) { top = p.y; }
		if (bottom == -1 || p.y > bottom) { bottom = p.y; }
		if (left == -1 || p.x < left) { left = p.x; }
		if (right == -1 || p.x > right) { right = p.x; }
	}

	int width = right + 1;
	int height = bottom + 1;

	int* sums = new int[width * height];

	// Find all areas and distance sums (this is slow)
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Point cur(x, y);

			int &sum = sums[y * width + x];
			sum = 0;

			Point* closest = nullptr;
			int closestDistance = 0;
			bool shared = false;

			bool inPoint = false;

			for (auto &p : points) {
				int distance = cur.distance(p);
				sum += distance;

				if (p == cur) {
					closest = &p;
					inPoint = true;
					if (sums[y * width + x - 1] > 10000) {
						sum = -1;
					}
					break;
				}

				if (closest == nullptr || distance < closestDistance) {
					closestDistance = distance;
					closest = &p;
					shared = false;
				} else if (distance == closestDistance) {
					shared = true;
				}
			}

			if (!inPoint && !shared) {
				closest->area++;

				if (closest->finite && (x == 0 || y == 0 || x == width - 1 || y == height - 1)) {
					closest->finite = false;
				}
			}
		}
	}

	// Part 1: Find the largest finite area
	int biggestArea = 0;
	for (auto &p : points) {
		if (p.finite && p.area > biggestArea) {
			biggestArea = p.area;
		}
	}
	printf("Part 1: %d\n", biggestArea);

	// Part 2: Find the region size of sum of point distances < 10000
	int part2 = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int &sum = sums[y * width + x];
			if (sum < 10000 && sum != -1) {
				part2++;
			}
		}
	}
	printf("Part 2: %d\n", part2);

	// Clean up
	delete[] sums;

	return 0;
}
