#include <cpu.h>

#include <memory.h>
#include <errno.h>

struct cpu_t *cpu_alloc(void)
{
	struct cpu_t *cpu = (struct cpu_t *)malloc(sizeof(struct cpu_t));
	if (!cpu)
		return NULL;

	uint16_t *regs = (uint16_t *)malloc(sizeof(uint16_t) * NUM_REGS);
	if (!regs)
		return NULL;

	memset(regs, 0, NUM_REGS);

	cpu->regs = gpr(regs);

	cpu->flag_reg = 0;

	cpu->pc = 0x50;

	cpu->bus = bus_alloc();
	if (!cpu->bus)
		return NULL;

	return cpu;
}

void cpu_free(struct cpu_t *cpu)
{
	free(cpu->regs.regs);
	bus_free(cpu->bus);
	free(cpu);
}

static int cpu_fetch(struct cpu_t *cpu, uint16_t *out)
{
	return bus_read(cpu->bus, cpu->pc, out);
}

int cpu_execute(struct cpu_t *cpu)
{
	uint16_t inst_raw;
	int result = cpu_fetch(cpu, &inst_raw);

	if (result < 0)
		return result;

	struct instruction_t inst = { .opcode = (inst_raw & INST_OPCODE) >> 11,
				      .rd = (inst_raw & INST_RD) >> 7,
				      .flags = (inst_raw & INST_FLAGS) >> 4,
				      .rs = (inst_raw & INST_RS) };
}

static void execute_general(struct cpu_t *cpu, struct instruction_t inst)
{
}

static void execute_imm(struct cpu_t *cpu, struct instruction_t inst)
{
}
