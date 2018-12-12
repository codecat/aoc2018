#include <cstdio>
#include <cstring>

#define S2_IMPL
#include "s2/s2list.h"

struct Node
{
	s2::list<Node> childNodes;
	s2::list<int> metadata;

	int readNumber(FILE* fh)
	{
		char buffer[6];
		int i = 0;

		while (!feof(fh)) {
			char c = fgetc(fh);
			if (c == ' ' || c == '\0') {
				buffer[i] = '\0';
				break;
			} else {
				buffer[i++] = c;
			}
		}

		return atoi(buffer);
	}

	void read(FILE* fh)
	{
		int numChildNodes = readNumber(fh);
		int numMetadataEntries = readNumber(fh);

		for (int i = 0; i < numChildNodes; i++) {
			childNodes.add().read(fh);
		}

		for (int i = 0; i < numMetadataEntries; i++) {
			metadata.add(readNumber(fh));
		}
	}

	int getSum()
	{
		int ret = 0;
		for (auto &n : childNodes) {
			ret += n.getSum();
		}
		for (int num : metadata) {
			ret += num;
		}
		return ret;
	}

	int getValue()
	{
		int ret = 0;

		if (childNodes.len() == 0) {
			for (int num : metadata) {
				ret += num;
			}
		} else {
			for (int num : metadata) {
				if (num > 0 && num <= (int)childNodes.len()) {
					ret += childNodes[num - 1].getValue();
				}
			}
		}

		return ret;
	}
};

Node g_root;

int main()
{
	// Read all nodes from file
	FILE* fh = fopen("input/8.input", "r");
	g_root.read(fh);
	fclose(fh);

	// Part 1: Walk the tree and get the sum of metadatas
	printf("Part 1: %d\n", g_root.getSum());

	// Part 2: Walk the tree and get the value of the root node
	printf("Part 2: %d\n", g_root.getValue());

	return 0;
}
