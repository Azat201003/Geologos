.PHONY: run clear

all: run

build/Geologos: CMakeLists.txt | build
	cd build && cmake ..
	cmake --build build --target Geologos -- -j $(nproc)

build:
	mkdir -p $@

run: build/Geologos
	$^

clear:
	rm -rf build/*
