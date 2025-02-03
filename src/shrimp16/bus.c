#include <bus.h>

#include <stdio.h>
#include <errno.h>
#include <memory.h>

struct bus_t *bus_alloc(void)
{
	struct bus_t *bus = (struct bus_t *)malloc(sizeof(struct bus_t));
	if (!bus)
		return NULL;

	uint16_t *memory = (uint16_t *)malloc(sizeof(uint16_t) * RAM_SIZE);
	if (!memory)
		return NULL;

	memset(memory, 0, RAM_SIZE);
	bus->ram = ram(memory);

	return bus;
}

void bus_free(struct bus_t *bus)
{
	free(ram_ptr(bus->ram));
	free(bus);
}

int bus_init_ram(struct bus_t *bus, FILE *file)
{
	fseek(file, 0L, SEEK_END);
	size_t len = ftell(file);
	rewind(file);
	uint16_t *binary = (uint16_t *)malloc(sizeof(uint16_t) * len);
	if (!binary)
		return -ENOMEM;

	fread(binary, len, 1, file);

	for (size_t i = 0; i < len; i++)
		ram_val(bus->ram, i) = (binary[i] >> 8) | (binary[i] << 8);

	return 0;
}

int bus_read(struct bus_t *bus, uint16_t addr, uint16_t *out)
{
	if (!out)
		return -EINVAL;

	if (addr >= RAM_BASE && addr <= RAM_END)
		*out = ram_val(bus->ram, addr - RAM_BASE);
	else
		return -EFAULT;

	return 0;
}

int bus_write(struct bus_t *bus, uint16_t addr, uint16_t val)
{
	if (addr >= RAM_BASE && addr <= RAM_END)
		ram_val(bus->ram, addr - RAM_BASE) = val;
	else
		return -EFAULT;

	return 0;
}
