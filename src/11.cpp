#include <cstdio>
#include <cstring>

struct Square
{
	int x = 0;
	int y = 0;
	int sum = -1;
	int size = 0;
};

static Square getBiggestSquare(int sz, int* fuel, int width, int height)
{
	Square ret;
	ret.size = sz;

	for (int y = 0; y < height - sz; y++) {
		for (int x = 0; x < width - sz; x++) {

			int sum = 0;
			for (int yy = 0; yy < sz; yy++) {
				for (int xx = 0; xx < sz; xx++) {
					sum += fuel[(y * width + x) + (yy * width + xx)];
				}
			}

			if (ret.sum == -1 || sum > ret.sum) {
				ret.sum = sum;
				ret.x = x;
				ret.y = y;
			}
		}
	}

	return ret;
}

int main()
{
	// Load serial number from file
	FILE* fh = fopen("input/11.input", "r");
	int serial;
	fscanf(fh, "%d", &serial);
	fclose(fh);

	// Make a grid of all fuel levels
	int width = 300;
	int height = 300;
	int* fuel = new int[width * height];

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = y * width + x;
			int &cell = fuel[index];

			int rack = x + 10;
			cell = rack * y;
			cell += serial;
			cell *= rack;
			int third = (cell % 1000) / 100;
			cell = third;
			cell -= 5;
		}
	}

	{
		// Part 1: Find biggest square of 3
		auto sq = getBiggestSquare(3, fuel, width, height);

		printf("Part 1: %d,%d\n", sq.x, sq.y);
	}

	{
		// Part 2: Find biggest square of any size between 3 and 299
		Square biggest;

		for (int sz = 3; sz < 299; sz++) {
			Square sq = getBiggestSquare(sz, fuel, width, height);
			if (biggest.sum == -1 || sq.sum > biggest.sum) {
				biggest = sq;
			}
		}

		printf("Part 2: %d,%d,%d\n", biggest.x, biggest.y, biggest.size);
	}

	delete[] fuel;

	return 0;
}
