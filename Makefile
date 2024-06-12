.PHONY: build clean test

COMPILER=clang++
STD=c++20
LINKER=clang++
WARNINGS=-Wall -Wpedantic

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
	${COMPILER} ${WARNINGS} -std=${STD} src/*/*.cpp src/main.cpp -o ${TARGET} -I./include -DDEBUG_BUILD

test:
	${COMPILER} ${WARNINGS} -std=${STD} tests/*/*.cpp src/*/*.cpp -o test_binary ${GTESTFLAGS} -I./include -O0 -DDEBUG_BUILD
	./test_binary
	${RM} test_binary
