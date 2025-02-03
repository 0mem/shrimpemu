#include <cpu.h>

#include <stdio.h>
#include <errno.h>

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage:\nshrimpemu <binary.bin>\n");
		return -EINVAL;
	}

	const char *binary_path = argv[1];
	FILE *binary_file = fopen(binary_path, "rb");
	if (!binary_file) {
		fprintf(stderr, "ERROR: Failed to open file %s", binary_path);
		return -ENOENT;
	}

	struct cpu_t *cpu = cpu_alloc();
	if (!cpu) {
		fclose(binary_file);
		return -ENOMEM;
	}

	int result = bus_init_ram(cpu->bus, binary_file);
	fclose(binary_file);

	if (result < 0) {
		cpu_free(cpu);
		return result;
	}

	result = cpu_execute(cpu);

	if (result < 0) {
		cpu_free(cpu);
		return result;
	}

	cpu_free(cpu);

	return 0;
}
