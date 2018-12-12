#include <cstdio>
#include <cstring>
#include <cstdint>

#define S2_IMPL
#include "s2/s2list.h"

struct Marble
{
	int number = 0;

	Marble* prev = this;
	Marble* next = this;

	Marble* back(int offset)
	{
		Marble* cur = this;
		for (int i = 0; i < offset; i++) {
			cur = cur->prev;
		}
		return cur;
	}
};

uint64_t calculateMaxScore(int numPlayers, int lastMarble)
{
	// Initialize player list
	s2::list<uint64_t> players;
	for (int i = 0; i < numPlayers; i++) {
		players.add(0);
	}

	auto currentIndex = new Marble;
	int currentPlayer = -1;

	int marbleCount = 0;

	for (int currentMarble = 1; currentMarble <= lastMarble; currentMarble++) {
		Marble* nextIndex = nullptr;

		if (currentMarble % 23 == 0) {
			// Add score and switch to item at -7
			auto takeIndex = currentIndex->back(7);

			players[currentPlayer] += currentMarble + takeIndex->number;

			nextIndex = takeIndex->next;

			takeIndex->prev->next = takeIndex->next;
			takeIndex->next->prev = takeIndex->prev;
			delete takeIndex;

		} else {
			// Insert after next
			auto placeAfter = currentIndex->next;
			auto placeBefore = placeAfter->next;

			nextIndex = new Marble;
			nextIndex->number = currentMarble;
			nextIndex->prev = placeAfter;
			nextIndex->next = placeBefore;

			placeAfter->next = nextIndex;
			placeBefore->prev = nextIndex;
		}

		currentIndex = nextIndex;
		currentPlayer = (currentPlayer + 1) % numPlayers;
	}

	// Get maximum score
	uint64_t maxScore = 0;
	for (uint64_t score : players) {
		if (score > maxScore) {
			maxScore = score;
		}
	}

	// Clean up
	auto cleanUp = currentIndex;
	do {
		auto next = cleanUp->next;
		delete cleanUp;
		cleanUp = next;
	} while (cleanUp != currentIndex);

	return maxScore;
}

int main()
{
	// Load values from file
	int numPlayers;
	int lastMarble;

	FILE* fh = fopen("input/9.input", "r");
	fscanf(fh, "%d players; last marble is worth %d points", &numPlayers, &lastMarble);
	fclose(fh);

	printf("Part 1: %lu\n", calculateMaxScore(numPlayers, lastMarble));
	printf("Part 2: %lu\n", calculateMaxScore(numPlayers, lastMarble * 100));

	return 0;
}
