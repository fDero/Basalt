.PHONY: build clean test

COMPILER=g++
STD=c++20
LINKER=clang++

MKDIR := mkdir -p
RM := rm -rf
GTESTFLAGS := -lgtest -lgtest_main -pthread

EXECUTABLE_EXTENSION := out
TARGET := basalt


clean:
	@$(RM) build
	@$(RM) ${TARGET}
	@$(RM) *.bt
	@$(RM) *.exe
	@$(RM) *.elf
	@$(RM) *.out

build:
	${COMPILER} -std=${STD} src/*/*.cpp src/main.cpp -Wall -o ${TARGET} -I./include

test:
	${COMPILER} -std=${STD} tests/*/*.cpp src/*/*.cpp -o test_binary ${GTESTFLAGS} -I./include -O0
	./test_binary
	${RM} test_binary
