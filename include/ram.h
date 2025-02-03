#ifndef RAM_H
#define RAM_H

#include <stdint.h>
#include <stdlib.h>

#define RAM_SIZE 0xffaf
#define RAM_BASE 0x50
#define RAM_END (RAM_BASE + RAM_SIZE)

/* The executable memory should only be accessed by bus, so we make them opaque. */
typedef struct {
	uint16_t *ram;
} ram_t;

/* Initializer, Accessor Macros */
#define ram(x) ((ram_t){ (x) })
#define ram_val(x, y) ((x).ram[y])
#define ram_ptr(x) ((x).ram)

#endif
