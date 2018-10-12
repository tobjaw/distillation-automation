build: src/*.c src/*.h
	cc -o bin/main src/*.c

run: bin/main
	bin/main

dev: src/*.c src/*.h
	make build && make run
