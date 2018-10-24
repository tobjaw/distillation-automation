cmake: *
	test ! -f compile_commands.json && tools/cmake/build.sh && tools/cmake/run-local.sh || exit 0

clang-format: *
	tools/clang-format/build.sh

format: src/*.c src/*.h
	make cmake; make clang-format && tools/clang-format/run-local.sh
