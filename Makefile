include config.mk

BINARY := build/shrimpemu

C_SOURCE_FILES := $(wildcard src/*.c) $(wildcard src/**/*.c)
C_OBJECT_FILES := $(patsubst src/%.c, build/%.o, $(C_SOURCE_FILES))

.PHONY: all clean run

all: BINARY

clean:
	@rm -r build/

run: BINARY
	./$(BINARY)

BINARY: $(C_OBJECT_FILES)
	@$(CC) -o $(BINARY) $(C_OBJECT_FILES) $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
