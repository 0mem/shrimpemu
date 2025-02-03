CC := gcc

LDFLAGS :=

INCLUDES := -I./include
CFLAGS := -O2 -Wall -Wextra -Wpedantic -std=gnu99 -g $(INCLUDES)
