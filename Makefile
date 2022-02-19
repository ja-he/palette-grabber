PROGNAME := palette-grabber

STD := -std=c++20
DEBUG_OPTIONS := -Og -g -fsanitize=address -fsanitize=leak
RELEASE_OPTIONS := -O3
OPTIONS := $(RELEASE_OPTIONS)
WRN := -Wall -Wextra -Wpedantic
LDFLAGS :=

SOURCES := $(wildcard src/*.cc)
OBJECTS := $(patsubst src/%.cc, .build/%.o, $(SOURCES))

all: .build $(PROGNAME)

$(PROGNAME): $(OBJECTS)
	g++ $(LDFLAGS) $(STD) $(OPTIONS) $(WRN) $^ -o $@

.build/%.o: src/%.cc
	g++ $(STD) $(OPTIONS) $(WRN) $^ -c -o $@

.build:
	mkdir .build

clean:
	rm -rf $(PROGNAME) .build/* core a.out
