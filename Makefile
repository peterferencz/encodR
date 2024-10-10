CC := gcc
CFLAGS := -g -lm -Wall -Werror

.PHONY: build run encode decode
.SILENT: encode decode
.DEFAULT: build

build: ./src/main.c
	${CC} ./src/main.c -o ./out/bin ${CFLAGS}

encode:
	./out/bin kodol ./out/test.txt ./out/encoded
	xxd -b ./out/encoded
	du --summarize --human-readable ./out/encoded

decode:
	./out/bin dekodol ./out/encoded ./decoded.txt
	diff ./out/test.txt ./decoded.txt