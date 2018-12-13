#include <cstdio>
#include <string>

#define S2_IMPL
#include "s2/s2list.h"

enum CellType
{
	ct_Empty,
	ct_Vert,
	ct_Hor,
	ct_Turn,
	ct_Cross,
};

enum Direction
{
	d_None = 0,
	d_Up = (1 << 0),
	d_Down = (1 << 1),
	d_Left = (1 << 2),
	d_Right = (1 << 3),
};

struct Cell
{
	CellType type = ct_Empty;
	int directions = d_None;
};

struct Cart
{
	int x = 0;
	int y = 0;
	int direction = d_None;
	int turnState = 0;
};

static int getInverseDirection(int direction)
{
	switch (direction) {
		case d_Up: return d_Down;
		case d_Down: return d_Up;
		case d_Left: return d_Right;
		case d_Right : return d_Left;
	}
	return d_None;
}

static int getRelativeDirection(int direction, int turn)
{
	switch (direction) {
		case d_Up: return turn; break;
		case d_Down:
			if (turn == d_Left) { return d_Right; }
			if (turn == d_Right) { return d_Left; }
			break;

		case d_Left:
			if (turn == d_Left) { return d_Down; }
			if (turn == d_Right) { return d_Up; }
			break;

		case d_Right:
			if (turn == d_Left) { return d_Up; }
			if (turn == d_Right) { return d_Down; }
			break;
	}
	return d_None;
}

int main()
{
	// Load grid and carts from file
	s2::list<Cell> grid;
	s2::list<Cart> carts;

	int width = -1;
	int height = 0;

#define GET_CELL(xx, yy) (grid[(yy) * width + (xx)])

	FILE* fh = fopen("input/13.input", "r");
	int current_x = 0;
	while (!feof(fh)) {
		char c = fgetc(fh);
		if (c == -1) {
			break;
		}
		if (c == '\r') {
			continue;
		}

		if (c == '\n') {
			if (width == -1) {
				width = (int)grid.len();
			}
			height++;
			current_x = 0;
			continue;
		}

		auto &newCell = grid.add();

		switch (c) {
			case ' ': newCell.type = ct_Empty; break;

			case '^': case 'v':
			case '|': newCell.type = ct_Vert; break;

			case '>': case '<':
			case '-': newCell.type = ct_Hor; break;

			case '+': newCell.type = ct_Cross; break;

			case '/':
			case '\\': newCell.type = ct_Turn; break;
		}

		if (c == '>' || c == '<' || c == '^' || c == 'v') {
			auto &newCart = carts.add();
			newCart.x = current_x;
			newCart.y = height;

			switch (c) {
				case '^': newCart.direction = d_Up; break;
				case 'v': newCart.direction = d_Down; break;
				case '<': newCart.direction = d_Left; break;
				case '>': newCart.direction = d_Right; break;
			}
		}

		current_x++;
	}
	fclose(fh);

	// Find directions for each turn
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			auto &c = GET_CELL(x, y);
			if (c.type != ct_Turn) {
				continue;
			}

			auto &up = y > 0 ? GET_CELL(x, y - 1) : c;
			auto &down = y < height - 1 ? GET_CELL(x, y + 1) : c;
			auto &left = x > 0 ? GET_CELL(x - 1, y) : c;
			auto &right = x < width - 1 ? GET_CELL(x + 1, y) : c;

			if (y > 0 && (up.type == ct_Vert || up.type == ct_Cross)) { c.directions |= d_Up; }
			if (y < height - 1 && (down.type == ct_Vert || down.type == ct_Cross)) { c.directions |= d_Down; }
			if (x > 0 && (left.type == ct_Hor || left.type == ct_Cross)) { c.directions |= d_Left; }
			if (x < width - 1 && (right.type == ct_Hor || right.type == ct_Cross)) { c.directions |= d_Right; }
		}
	}

	// Begin stepping through each iteration until a collision occurs
	bool foundFirstCrash = false;
	while (carts.len() > 1) {
		carts.sort([](const void* pa, const void* pb) {
			auto &a = *(Cart*)pa;
			auto &b = *(Cart*)pb;

			int da = a.x + a.y;
			int db = b.x + b.y;

			if (da > db) { return 1; }
			if (da < db) { return -1; }
			return 0;
		});

		for (int i = 0; i < (int)carts.len(); i++) {
			auto &cart = carts[i];

			switch (cart.direction) {
				case d_Up: cart.y--; break;
				case d_Down: cart.y++; break;
				case d_Left: cart.x--; break;
				case d_Right: cart.x++; break;
			}

			auto &cell = GET_CELL(cart.x, cart.y);
			if (cell.type == ct_Cross) {
				switch (cart.turnState) {
					case 0: cart.direction = getRelativeDirection(cart.direction, d_Left); break;
					case 2: cart.direction = getRelativeDirection(cart.direction, d_Right); break;
				}

				if (++cart.turnState == 3) {
					cart.turnState = 0;
				}

			} else if (cell.type == ct_Turn) {
				cart.direction = cell.directions & ~(getInverseDirection(cart.direction));
			}

			// Check if we collided into any other cart
			for (int j = 0; j < (int)carts.len(); j++) {
				if (j == i) {
					continue;
				}

				auto &otherCart = carts[j];
				if (otherCart.x == cart.x && otherCart.y == cart.y) {
					if (!foundFirstCrash) {
						printf("Part 1: %d,%d\n", cart.x, cart.y);
						foundFirstCrash = true;
					}

					carts.remove(j--);
					if (j < i) {
						i--;
					}
					carts.remove(i--);
					break;
				}
			}
		}
	}

	printf("Part 2: %d,%d\n", carts[0].x, carts[0].y);

	return 0;
}
