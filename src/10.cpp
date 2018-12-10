#include <cstdio>
#include <cstring>

#define S2_IMPL
#include "s2/s2list.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

struct Vector
{
	int x;
	int y;

	Vector()
	{
		x = 0;
		y = 0;
	}

	Vector(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	Vector& operator +=(const Vector &v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector& operator -=(const Vector &v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
};

struct Point
{
	Vector position;
	Vector velocity;
};

int main()
{
	// Load points from file
	s2::list<Point> points;

	FILE* fh = fopen("input/10.input", "r");
	char buffer[52];
	while (fgets(buffer, 52, fh)) {
		size_t len = strlen(buffer);
		buffer[--len] = '\0';
		if (len == 0) {
			break;
		}

		int x, y, vx, vy;
		int parsed = sscanf(buffer, "position=<%d, %d> velocity=<%d, %d>", &x, &y, &vx, &vy);

		auto &p = points.add();
		p.position.x = x;
		p.position.y = y;
		p.velocity.x = vx;
		p.velocity.y = vy;
	}
	fclose(fh);

	// Find the point at which the height is the smallest
	int height = -1;
	int iterations = 0;
	while (true) {
		int top, bottom;
		top = bottom = -1;

		for (auto &p : points) {
			p.position += p.velocity;

			if (top == -1 || p.position.y < top) { top = p.position.y; }
			if (bottom == -1 || p.position.y > bottom) { bottom = p.position.y; }
		}

		int h = bottom - top;

		if (height == -1 || h < height) {
			height = h;

			// Part 2: Count iterations
			iterations++;
		} else {
			break;
		}
	}

	// Go back one step and find bounds
	int bound_left = -1;
	int bound_right = -1;
	int bound_top = -1;
	for (auto &p : points) {
		p.position -= p.velocity;

		if (bound_left == -1 || p.position.x < bound_left) { bound_left = p.position.x; }
		if (bound_right == -1 || p.position.x > bound_right) { bound_right = p.position.x; }
		if (bound_top == -1 || p.position.y < bound_top) { bound_top = p.position.y; }
	}
	int width = bound_right - bound_left + 1;
	height++;

	// Part 1: Write image
	size_t imgSize = width * height * 3;
	unsigned char* img = new unsigned char[imgSize];
	memset(img, 0, imgSize);
	for (auto &p : points) {
		int x = p.position.x - bound_left;
		int y = p.position.y - bound_top;
		int index = y * width * 3 + x * 3;
		img[index] = 0xFF;
	}
	stbi_write_png("10.png", width, height, 3, img, width * 3);
	delete[] img;

	printf("Part 1: See 10.png\n");
	printf("Part 2: %d\n", iterations);

	return 0;
}
