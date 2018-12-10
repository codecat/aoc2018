CFLAGS = -std=c++11 -lstdc++ -lm -O3 -Wno-unused-result

def:
	@echo "Use make <number>."

all: 1 2 3 4 5 6 7 8 9 10

1:
	gcc src/1.cpp $(CFLAGS) -o 1.exe
2:
	gcc src/2.cpp $(CFLAGS) -o 2.exe
3:
	gcc src/3.cpp $(CFLAGS) -o 3.exe
4:
	gcc src/4.cpp $(CFLAGS) -o 4.exe
5:
	gcc src/5.cpp $(CFLAGS) -o 5.exe
6:
	gcc src/6.cpp $(CFLAGS) -o 6.exe
7:
	gcc src/7.cpp $(CFLAGS) -o 7.exe
8:
	gcc src/8.cpp $(CFLAGS) -o 8.exe
9:
	gcc src/9.cpp $(CFLAGS) -o 9.exe
10:
	gcc src/10.cpp $(CFLAGS) -o 10.exe
