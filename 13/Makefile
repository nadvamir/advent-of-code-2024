CC=clang
PROGRAM_1=claw_contraption
PROGRAM_2=claw_contraption2

all: p1 p2

p1:
	$(CC) --std=c++23 -lc++ $(PROGRAM_1).cpp -o $(PROGRAM_1).exe
	cat data.in | ./$(PROGRAM_1).exe

p2:
	$(CC) --std=c++23 -lc++ $(PROGRAM_2).cpp -o $(PROGRAM_2).exe
	cat data.in | ./$(PROGRAM_2).exe
