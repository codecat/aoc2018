#include <cstdio>
#include <cstring>

#define S2_IMPL
#include "s2/s2list.h"
#include "s2/s2string.h"

class Date
{
public:
	int month;
	int day;
	int hour;
	int minute;

public:
	int cmp(const Date &d)
	{
		if (month < d.month) { return -1; } else if (month > d.month) { return 1; }
		if (day < d.day) { return -1; } else if (day > d.day) { return 1; }

		if (hour < d.hour) { return -1; } else if (hour > d.hour) { return 1; }
		if (minute < d.minute) { return -1; } else if (minute > d.minute) { return 1; }

		return 0;
	}

	int datestamp()
	{
		return (month * 31) + day;
	}
};

enum EventType
{
	et_Begin,
	et_End,
};

class Event
{
public:
	Date date;
	int guard;
	EventType type;
};

class Guard
{
public:
	int id;
	int timeSlept = 0;
	int sleepDays[60] = { 0 };

	Event* lastEvent = nullptr;
	s2::list<Event*> events;

public:
	void reset()
	{
		if (lastEvent != nullptr && lastEvent->type == et_Begin) {
			timeSlept += 60 - lastEvent->date.minute;
		}
		lastEvent = nullptr;
	}
};

class GuardManager
{
public:
	s2::list<Guard*> m_guards;

public:
	GuardManager() { }
	~GuardManager()
	{
		for (auto g : m_guards) {
			delete g;
		}
	}

	Guard* get(int id)
	{
		for (auto g : m_guards) {
			if (g->id == id) {
				return g;
			}
		}

		Guard* &ret = m_guards.add();
		ret = new Guard;
		ret->id = id;
		return ret;
	}

	void reset()
	{
		for (auto g : m_guards) {
			g->reset();
		}
	}
};

int main()
{
	// Read events from file
	s2::list<Event> events;

	FILE* fh = fopen("input/4.input", "r");
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

		auto &e = events.add();
		int year; // Not relevant, always 1518
		sscanf(buffer, "[%d-%d-%d %d:%d] ",
			&year, &e.date.month, &e.date.day,
			&e.date.hour, &e.date.minute
		);

		s2::stringsplit parse(buffer + 19, " ");
		s2::string cmd = parse[0];

		e.guard = -1;
		if (cmd == "falls") {
			e.type = et_End;
		} else if (cmd == "wakes") {
			e.type = et_Begin;
		} else if (cmd == "Guard") {
			e.type = et_Begin;
			sscanf(parse[1], "#%d", &e.guard);
		}
	}
	fclose(fh);

	// Sort the events in chronological order
	events.sort([](const void* pa, const void* pb) {
		auto a = (Event*)pa;
		auto b = (Event*)pb;
		return a->date.cmp(b->date);
	});

	// Assert that we have a guard ID in the first event
	if (events[0].guard == -1) {
		printf("ERROR: We don't have a guard ID in the first event!\n");
		return 1;
	}

	// Map out the schedules for each guard
	GuardManager guards;
	Guard* currentGuard = nullptr;
	int lastDatestamp = -1;

	for (auto &e : events) {
		int datestamp = e.date.datestamp();
		if (lastDatestamp != datestamp) {
			lastDatestamp = datestamp;
			guards.reset();
		}

		if (e.guard != -1) {
			currentGuard = guards.get(e.guard);
		}

		int timeSinceLastEvent = 0;
		if (currentGuard->lastEvent != nullptr) {
			timeSinceLastEvent = e.date.minute - currentGuard->lastEvent->date.minute;
		}

		if (e.date.hour == 0 && e.type == et_Begin) {
			currentGuard->timeSlept += timeSinceLastEvent;
		}

		currentGuard->lastEvent = &e;
		currentGuard->events.add(&e);
	}

	for (auto g : guards.m_guards) {
		int lastSleepDay = g->events[0]->date.datestamp();
		Event* lastEvent = nullptr;

		for (auto e : g->events) {
			int datestamp = e->date.datestamp();
			if (datestamp != lastSleepDay) {
				lastSleepDay = datestamp;
				lastEvent = nullptr;
			}

			if (e->type == et_Begin && lastEvent != nullptr && lastEvent->type == et_End) {
				for (int i = lastEvent->date.minute; i < e->date.minute; i++) {
					g->sleepDays[i]++;
				}
			}

			lastEvent = e;
		}
	}

	{
		// Part 1: Find the guard asleep the most
		Guard* mostSleepyGuard = nullptr;
		for (auto g : guards.m_guards) {
			if (mostSleepyGuard == nullptr || g->timeSlept > mostSleepyGuard->timeSlept) {
				mostSleepyGuard = g;
			}
		}

		// Find what minute that particular guard has been sleeping the most each day
		//int maxIndex = getMaxIndex(mostSleepyGuard->sleepDays, 60);
		int maxIndex = 0;
		for (int i = 1; i < 60; i++) {
			if (mostSleepyGuard->sleepDays[i] > mostSleepyGuard->sleepDays[maxIndex]) {
				maxIndex = i;
			}
		}

		printf("Part 1: %d * %d = %d\n", mostSleepyGuard->id, maxIndex, mostSleepyGuard->id * maxIndex);
	}

	// Part 2: Of all the guards, find the guard that is most frequently asleep on the same minute
	{
		int maxNum = 0;
		int maxIndex = 0;
		Guard* maxGuard = nullptr;

		for (auto g : guards.m_guards) {
			for (int i = 0; i < 60; i++) {
				int num = g->sleepDays[i];
				if (num > maxNum) {
					maxNum = num;
					maxIndex = i;
					maxGuard = g;
				}
			}
		}

		printf("Part 2: %d * %d = %d\n", maxGuard->id, maxIndex, maxGuard->id * maxIndex);
	}

	return 0;
}
