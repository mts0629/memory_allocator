.PHONY: format clean

all: format
	cmake -S . -B build
	cmake --build build

format:
	clang-format -style=file:../.clang-format -i src/*.h src/*.c
	clang-format -style=file:../.clang-format -i *.c

clean:
	cmake --build build --target clean
