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

dir_guard=@mkdir -p $(@D)

# Generate object files
$(OUTPATH)/obj/%.o : $(SRCPATH)/%.c
	$(dir_guard)
	$(CC) -I$(HEADERPATH) $(CFLAGS) -c $< -o $@

# Link object files
$(OUTPATH)/encodr: $(OBJECTFILES)
	$(CC) $(OBJECTFILES) -o $(OUTPATH)/encodr

.PHONY: build
build: $(OUTPATH)/encodr

.PHONY: clean
clean: FORCE
# c object files
	rm ./out/obj/*.o
	rm ./out/encodr
# doxygen files
	$(MAKE) -C ./docs/working clean
	rm -r ./docs/working

.PHONY: docs
docs: ./docs/Documentation.pdf ./docs/Usermanual.pdf

./docs/Usermanual.pdf: ./docs/standalone/userman.tex
	$(dir_guard)
	pdflatex -halt-on-error -output-directory=./docs/working ./docs/standalone/userman.tex
	cp ./docs/working/userman.pdf ./docs/Usermanual.pdf

./docs/Documentation.pdf:
	$(dir_guard)
	doxygen Doxyfile
	cd ./docs/working
	$(MAKE) -C ./docs/working
	cp ./docs/working/refman.pdf ./docs/Documentation.pdf

.PHONY: encode
encode:
	./out/encodr kodol --bemenet ./out/test.txt --kimenet ./out/encoded --kodtabla --statisztika
# 	echo
# 	xxd -b ./out/encoded | (head ; echo ; tail)
# 	du --summarize --human-readable ./out/encoded

.PHONY: decode
decode:
	./out/encodr dekodol --bemenet ./out/encoded --kimenet ./out/decoded.txt --kodtabla --statisztika
#	echo
#	cat ./out/decoded.txt
#	echo
#	diff -q ./out/test.txt ./out/decoded.txt

FORCE: ;

.PHONY: test
test:
	rm ./out/random.data
	head -c 100000 < /dev/urandom > ./out/random.data
	./out/encodr kodol --bemenet ./out/random.data --kimenet ./out/encoded
	./out/encodr dekodol --bemenet ./out/encoded --kimenet ./out/decoded.txt
	diff -q ./out/random.data ./out/decoded.txt
	echo "Test passed!"
