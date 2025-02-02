#include <cpu.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage:\nshrimpemu <binary.bin>\n");
        exit(EXIT_FAILURE);
    }

    const char* binary_path = argv[1];
    FILE* binary_file = fopen(binary_path, "rb");
    if (!binary_file) {
        fprintf(stderr, "ERROR: Failed to open file %s", binary_path);
        exit(EXIT_FAILURE);
    }
    fseek(binary_file, 0L, SEEK_END);
    size_t binary_size = ftell(binary_file);
    rewind(binary_file);
    uint16_t *binary = (uint16_t *)malloc(sizeof(uint16_t) * binary_size);
    fread(binary, binary_size, 1, binary_file);

    cpu_t *cpu = cpu_new();

    bus_initialize_ram(cpu->bus, binary, binary_size);
    cpu->program_counter = 0x50;
    
    execute(cpu);
    printf("%d\n", register_read(&cpu->registers, 1));

    cpu_free(cpu);

    return 0;
}