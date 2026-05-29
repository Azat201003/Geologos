all: bld run

bld:
	mkdir -p build && \
	cmake build -S . -B ./build -DCMAKE_BUILD_TYPE=Debug && \
	make -j -C build

run:
	build/Geologos
