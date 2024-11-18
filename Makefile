BUILD := debug # use make BUILD=release
CC := gcc

# Setup c flags based on BUILD
CFLAGS := -lm -Wall -Werror
ifeq ($(BUILD),release)
    CFLAGS += -O2
else
    CFLAGS +=  -g -ggdb -O0 -D DEBUG
endif

SRCPATH := src
HEADERPATH := lib
OUTPATH := out

SOURCEFILES := $(wildcard $(SRCPATH)/*.c)
OBJECTFILES := $(patsubst $(SRCPATH)/%.c, $(OUTPATH)/obj/%.o, $(SOURCEFILES))
HEADERFILES := $(wildcard $(HEADERPATH)/*.h)

# Generate object files
$(OUTPATH)/obj/%.o : $(SRCPATH)/%.c
	$(CC) -I$(HEADERPATH) $(CFLAGS) -c $< -o $@

# Link object files
$(OUTPATH)/bin: $(OBJECTFILES)
	$(CC) $(OBJECTFILES) -o $(OUTPATH)/bin

.PHONY: build
build: $(OUTPATH)/bin

.PHONY: clean
clean: FORCE
# c object files
	rm ./out/obj/*.o
	rm ./out/bin
# doxygen files
	$(MAKE) -C ./docs/working clean
	rm -r ./docs/working

.PHONY: docs
docs: ./docs/Documentation.pdf

./docs/Documentation.pdf: FORCE
	doxygen Doxyfile
	cd ./docs/working
	$(MAKE) -C ./docs/working
	cp ./docs/working/refman.pdf ./docs/Documentation.pdf

.PHONY: encode
encode:
	./out/bin kodol --bemenet ./out/test.txt --kimenet ./out/encoded --kodtabla --statisztika
# 	echo
# 	xxd -b ./out/encoded | (head ; echo ; tail)
# 	du --summarize --human-readable ./out/encoded

.PHONY: decode
decode:
	./out/bin dekodol --bemenet ./out/encoded --kimenet ./out/decoded.txt --kodtabla --statisztika
#	echo
#	cat ./out/decoded.txt
#	echo
#	diff -q ./out/test.txt ./out/decoded.txt

FORCE: ;

.PHONY: test
test:
	rm ./out/random.data
	head -c 100000 < /dev/urandom > ./out/random.data
	./out/bin kodol --bemenet ./out/random.data --kimenet ./out/encoded
	./out/bin dekodol --bemenet ./out/encoded --kimenet ./out/decoded.txt
	diff -q ./out/random.data ./out/decoded.txt
	echo "Test passed!"

# zip: ./docs/Documentation.pdf ./src/*
