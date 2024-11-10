CC := gcc
CFLAGS := -lm -Wall -Werror
DEBUGFLAGS := -g -ggdb -D DEBUG

.PHONY: build debug docs encode decode clean
.SILENT: build debug encode decode docs test
.DEFAULT: build

build: ./src/main.c
	${CC} ${CFLAGS} ./src/main.c -o ./out/bin

debug:
	${CC} ${CFLAGS} ${DEBUGFLAGS} ./src/main.c -o ./out/bin

# docs: Doxyfile
# 	doxygen Doxyfile
# 	cd ./docs/working
# 	$(MAKE) -C ./docs/working
# 	cp ./docs/working/refman.pdf ./docs/Documentation.pdf
docs:
	mkdir -p ./docs/out
	pdflatex -halt-on-error -output-directory ./docs/out -output-format pdf ./docs/main.tex

encode:
	./out/bin kodol --bemenet ./out/test.txt --kimenet ./out/encoded --kodtabla --statisztika
	echo
	xxd -b ./out/encoded | (head ; echo ; tail)
	du --summarize --human-readable ./out/encoded

decode:
	./out/bin dekodol --bemenet ./out/encoded --kimenet ./out/decoded.txt --kodtabla --statisztika
	echo
	cat ./out/decoded.txt
	echo
	diff -q ./out/test.txt ./out/decoded.txt

test:
	rm ./out/random.data
	head -c 1 < /dev/urandom > ./out/random.data
	./out/bin kodol --bemenet ./out/random.data --kimenet
	./out/bin dekodol --bemenet ./out/encoded --kimenet ./out/decoded.txt
	diff -q ./out/random.data ./out/decoded.txt

clean:
	$(MAKE) -C ./docs/working clean
	rm -r ./docs/working