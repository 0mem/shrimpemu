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
		return -EINVAL;
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
		if (result == -ENOMEM)
			fprintf(stderr, "Memory Allocation Failed\n");
		else if (result == -EINVAL)
			fprintf(stderr, "Invalid Argument\n");
		else if (result == -EFAULT)
			fprintf(stderr, "Invalid Address\n");

		return result;
	}

	result = cpu_execute(cpu);
	printf("%d\n", gpr_val(cpu->regs, 1));

	cpu_free(cpu);

	return 0;
}
