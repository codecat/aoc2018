CFLAGS = -std=c++11 -lstdc++ -lm -O3 -Wno-unused-result

def:
	@echo "Use make <number>."

all: 1 2 3 4 5 6 7 8 9 10 11 12

1:
	gcc src/01.cpp $(CFLAGS) -o 1.exe
2:
	gcc src/02.cpp $(CFLAGS) -o 2.exe
3:
	gcc src/03.cpp $(CFLAGS) -o 3.exe
4:
	gcc src/04.cpp $(CFLAGS) -o 4.exe
5:
	gcc src/05.cpp $(CFLAGS) -o 5.exe
6:
	gcc src/06.cpp $(CFLAGS) -o 6.exe
7:
	gcc src/07.cpp $(CFLAGS) -o 7.exe
8:
	gcc src/08.cpp $(CFLAGS) -o 8.exe
9:
	gcc src/09.cpp $(CFLAGS) -o 9.exe
10:
	gcc src/10.cpp $(CFLAGS) -o 10.exe
11:
	gcc src/11.cpp $(CFLAGS) -o 11.exe
12:
	gcc src/12.cpp $(CFLAGS) -o 12.exe
