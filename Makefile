.PHONY: run build debug clear

all: run

build/Geologos: build

build: CMakeLists.txt
	mkdir -p $@
	cd build && cmake .. && make -j

run: build/Geologos
	$^

clear:
	rm -rf build/*

debug: build/Geologos
	gdb build/Geologos
