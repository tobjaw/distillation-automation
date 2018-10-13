build: src/*.c src/*.h
	cc -o bin/main src/*.c

run: bin/main
	bin/main

dev: src/*.c src/*.h
	make build && make run

docs: src/*.c src/*.h docs/Doxyfile
	doxygen docs/Doxyfile

cmake: *
	test ! -f compile_commands.json && tools/cmake/build.sh && tools/cmake/run-local.sh || exit 0

clang-format: *
	tools/clang-format/build.sh

format: src/*.c src/*.h
	make cmake; make clang-format && tools/clang-format/run-local.sh
