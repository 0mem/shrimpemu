#ifndef BUS_H
#define BUS_H

#include <ram.h>

#include <stdio.h>

struct bus_t {
	ram_t ram;
};

struct bus_t *bus_alloc(void);
void bus_free(struct bus_t *bus);

int bus_init_ram(struct bus_t *bus, FILE *bin);

int bus_read(struct bus_t *bus, uint16_t addr, uint16_t *out);
int bus_write(struct bus_t *bus, uint16_t addr, uint16_t val);

#endif
