CFLAGS = -std=c++11 -lstdc++ -O3 -Wno-unused-result

all:
	@echo "Use make <number>."

1:
	gcc 1.cpp $(CFLAGS) -o 1.exe
2:
	gcc 2.cpp $(CFLAGS) -o 2.exe
3:
	gcc 3.cpp $(CFLAGS) -o 3.exe
4:
	gcc 4.cpp $(CFLAGS) -o 4.exe
5:
	gcc 5.cpp $(CFLAGS) -o 5.exe
