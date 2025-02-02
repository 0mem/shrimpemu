#include <ram.h>

ram_t * ram_new(void) {
    ram_t *ram = (ram_t *)malloc(sizeof(ram_t));
    memset(ram->memory, 0, RAM_SIZE);
    ram->code_size = 0;

    return ram;
}

void ram_free(ram_t *ram) {
    free(ram);
}

void ram_initialize(ram_t *ram, uint16_t *binary, size_t binary_size) {
    ram->code_size = binary_size;
    memcpy(ram, binary, binary_size);
}

uint16_t ram_read(ram_t *ram, uint16_t address) {
    size_t index = address - RAM_BASE;
    return (ram->memory[index] >> 8) | (ram->memory[index] << 8);
}

void ram_write(ram_t *ram, uint16_t address, uint16_t value) {
    size_t index = address - RAM_BASE;
    ram->memory[index] = value;
}
