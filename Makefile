CC := gcc
CFLAGS := -lm -Wall -Werror
DEBUGFLAGS := -g -ggdb -D DEBUG

# .SILENT: build debug encode decode docs test
# .PHONY: build debug docs encode decode clean zip
# .DEFAULT: build

# build: ./out/bin
# debug: ./out/debugbin
# docs: ./docs/Documentation.pdf

# ./out/bin: $(wildcard ./src/*.c)
# 	${CC} ${CFLAGS} ./src/main.c -o ./out/bin

# ./out/debugbin: $(wildcard ./src/*.c) $(wildcard ./src/debug/*)
# 	${CC} ${CFLAGS} ${DEBUGFLAGS} ./src/main.c -o ./out/debugbin

SRCPATH := src
OUTPATH := out

SOURCEFILES := $(wildcard $(SRCPATH)/*.c)
OBJECTFILES := $(patsubst $(SRCPATH)/%.c, $(OUTPATH)/obj/%.o, $(SOURCEFILES))

$(OUTPATH)/obj/%.o : $(SRCPATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPATH)/bin: $(OBJECTFILES)
	$(CC) $(OBJECTFILES) -o $(OUTPATH)/bin

.PHONY: build
build: $(OUTPATH)/bin

# ./docs/Documentation.pdf: Doxyfile
# 	doxygen Doxyfile
# 	cd ./docs/working
# 	$(MAKE) -C ./docs/working
# 	cp ./docs/working/refman.pdf ./docs/Documentation.pdf

# encode:
# 	./out/bin kodol --bemenet ./out/test.txt --kimenet ./out/encoded --kodtabla --statisztika
# 	echo
# 	xxd -b ./out/encoded | (head ; echo ; tail)
# 	du --summarize --human-readable ./out/encoded

# decode:
# 	./out/bin dekodol --bemenet ./out/encoded --kimenet ./out/decoded.txt --kodtabla --statisztika
# 	echo
# 	cat ./out/decoded.txt
# 	echo
# 	diff -q ./out/test.txt ./out/decoded.txt

# test:
# 	rm ./out/random.data
# 	head -c 100000 < /dev/urandom > ./out/random.data
# 	./out/bin kodol --bemenet ./out/random.data --kimenet ./out/encoded
# 	./out/bin dekodol --bemenet ./out/encoded --kimenet ./out/decoded.txt
# 	diff -q ./out/random.data ./out/decoded.txt
# 	echo "Test passed!"

# clean:
# 	$(MAKE) -C ./docs/working clean
# 	rm -r ./docs/working

# zip: ./docs/Documentation.pdf ./src/*
