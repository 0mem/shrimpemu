#ifndef BUS_H
#define BUS_H

#include <ram.h>

#include <stdio.h>

/*
	SHRIMP Memory Structure
	---------------------------------------------------------------------------
	  SHRIMP defines a memory, consisting 65536 (2^16) addressable,
	memory locations.

	0x0-0xf: Memory-mapped Ports
	0x10-0x4f: Interrupt Descriptor Table (IDT)
	0x50-0xffff: Executable Memory
	---------------------------------------------------------------------------
 */
struct bus_t {
	/* TODO: Memory-mapped Ports and IDT. (The architecture is not fully developed yet.) */
	ram_t ram; /* Executable Memory (mapping 0x50-0xffff) */
};

struct bus_t *bus_alloc(void);
void bus_free(struct bus_t *bus);

int bus_init_ram(struct bus_t *bus, FILE *bin);

int bus_read(struct bus_t *bus, uint16_t addr, uint16_t *out);
int bus_write(struct bus_t *bus, uint16_t addr, uint16_t val);

#endif
