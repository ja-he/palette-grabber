PROGNAME := palette-grabber

STD := -std=c++20
OPT := -O3
WRN := -Wall -Wextra -Wpedantic
LDFLAGS :=

SOURCES := $(wildcard src/*.cc)
OBJECTS := $(patsubst src/%.cc, .build/%.o, $(SOURCES))

all: .build $(PROGNAME)

$(PROGNAME): $(OBJECTS)
	g++ $(LDFLAGS) $(STD) $(OPT) $(WRN) $^ -o $@

.build/%.o: src/%.cc
	g++ $(STD) $(OPT) $(WRN) $^ -c -o $@

.build:
	mkdir .build

clean:
	rm -rf $(PROGNAME) .build/* core a.out
