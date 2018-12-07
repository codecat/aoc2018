#include <cstdio>
#include <cstring>

#define S2_IMPL
#include "s2/s2list.h"

struct Step
{
	char letter;
	s2::list<char> dependencies;

	bool operator ==(char c)
	{
		return letter == c;
	}

	int time()
	{
		return 60 + (letter - 'A' + 1);
	}
};

s2::list<Step> g_steps;

static Step &getStep(char letter)
{
	for (auto &step : g_steps) {
		if (step.letter == letter) {
			return step;
		}
	}

	auto &newStep = g_steps.add();
	newStep.letter = letter;
	return newStep;
}

struct Worker
{
	int endTime = 0;
	char work = '\0';
};

int main()
{
	// Load all steps
	FILE* fh = fopen("input/7.input", "r");
	char buffer[52];
	while (fgets(buffer, 52, fh)) {
		size_t len = strlen(buffer);
		buffer[--len] = '\0';
		if (len == 0) {
			break;
		}

		char c, dep;
		sscanf(buffer, "Step %c must be finished before step %c can begin.", &dep, &c);

		auto &step = getStep(c);
		getStep(dep); // make sure the dependency exists, too
		step.dependencies.add(dep);
	}

	g_steps.sort([](const void* pa, const void* pb) {
		auto &a = *(const Step*)pa;
		auto &b = *(const Step*)pb;

		if (a.letter > b.letter) { return 1; }
		if (a.letter < b.letter) { return -1; }
		return 0;
	});

	for (auto &step : g_steps) {
		step.dependencies.sort([](const void* pa, const void* pb) {
			char a = *(const char*)pa;
			char b = *(const char*)pb;

			if (a > b) { return 1; }
			if (a < b) { return -1; }
			return 0;
		});
	}

	// Part 1: Find the correct order of all the steps
	s2::list<char> order;
	{
		s2::list<Step> todo;
		for (auto &step : g_steps) {
			todo.add(step);
		}
		while (todo.len() > 0) {
			for (size_t i = 0; i < todo.len(); i++) {
				auto &step = todo[i];

				bool ready = true;
				for (auto dep : step.dependencies) {
					if (order.indexof(dep) == -1) {
						ready = false;
						break;
					}
				}

				if (ready) {
					order.add(step.letter);
					todo.remove(i);
					break;
				}
			}
		}

		printf("Part 1: ");
		for (auto c : order) {
			putchar(c);
		}
		putchar('\n');
	}

	{
		// Part 2: Find the time required for 5 workers to work on the steps
		s2::list<Worker> workers;
		for (int i = 0; i < 5; i++) {
			workers.add();
		}

		s2::list<Step> todo;
		for (char c : order) {
			todo.add(getStep(c));
		}

		s2::list<char> done;

		int currentTime = 0;
		while (todo.len() > 0) {
			for (size_t i = 0; i < todo.len(); i++) {
				auto &step = todo[i];
				int time = step.time();

				bool canWorkOn = true;
				for (auto dep : step.dependencies) {
					if (done.indexof(dep) == -1) {
						canWorkOn = false;
						break;
					}
				}

				if (!canWorkOn) {
					continue;
				}

				Worker* worker = nullptr;
				for (auto &w : workers) {
					if (currentTime >= w.endTime) {
						worker = &w;
					}
				}
				if (worker == nullptr) {
					break;
				}

				worker->endTime = currentTime + time;
				worker->work = step.letter;

				todo.remove(i--);
			}

			int minTime = -1;
			for (auto &w : workers) {
				if (w.work != '\0' && (minTime == -1 || w.endTime < minTime)) {
					minTime = w.endTime;
				}
			}

			currentTime = minTime;

			for (auto &w : workers) {
				if (w.work == '\0' || currentTime < w.endTime) {
					continue;
				}

				done.add(w.work);
				w.work = '\0';
			}
		}

		int totalTime = 0;
		for (auto &w : workers) {
			if (w.endTime > totalTime) {
				totalTime = w.endTime;
			}
		}

		printf("Part 2: %d seconds\n", totalTime);
	}

	return 0;
}
