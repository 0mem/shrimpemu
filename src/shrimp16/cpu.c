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

	cpu->flag_reg = freg(0);

	cpu->pc = pc(RAM_BASE);

	cpu->bus = bus_alloc();
	if (!cpu->bus)
		return NULL;

	return cpu;
}

void cpu_free(struct cpu_t *cpu)
{
	free(gpr_ptr(cpu->regs));
	bus_free(cpu->bus);
	free(cpu);
}

static int cpu_fetch(struct cpu_t *cpu, uint16_t *out)
{
	return bus_read(cpu->bus, pc_val(cpu->pc), out);
}

static void execute_general(struct cpu_t *cpu, instruction_t inst)
{
	switch (inst.opcode) {
	case OPCODE_ADD:
		gpr_val(cpu->regs, inst.rd) += gpr_val(cpu->regs, inst.rs);
		break;
	case OPCODE_SUB:
		gpr_val(cpu->regs, inst.rd) -= gpr_val(cpu->regs, inst.rs);
		break;
	case OPCODE_SHA:
		gpr_val(cpu->regs, inst.rd) >>= gpr_val(cpu->regs, inst.rs);
		break;
	case OPCODE_AND:
		gpr_val(cpu->regs, inst.rd) &= gpr_val(cpu->regs, inst.rs);
		break;
	case OPCODE_OR:
		gpr_val(cpu->regs, inst.rd) |= gpr_val(cpu->regs, inst.rs);
		break;
	case OPCODE_XOR:
		gpr_val(cpu->regs, inst.rd) ^= gpr_val(cpu->regs, inst.rs);
		break;
	case OPCODE_NOT:
		gpr_val(cpu->regs, inst.rd) = ~gpr_val(cpu->regs, inst.rs);
		break;
	case OPCODE_MOV:
		gpr_val(cpu->regs, inst.rd) = gpr_val(cpu->regs, inst.rs);
		break;
	}

	pc_move(cpu->pc, 1);
}

static void execute_general_imm(struct cpu_t *cpu, instruction_t inst)
{
	pc_move(cpu->pc, 1);
	uint16_t imm;
	cpu_fetch(cpu, &imm);

	switch (inst.opcode) {
	case OPCODE_ADD:
		gpr_val(cpu->regs, inst.rd) += imm;
		break;
	case OPCODE_SUB:
		gpr_val(cpu->regs, inst.rd) -= imm;
		break;
	case OPCODE_SHA:
		gpr_val(cpu->regs, inst.rd) >>= (int16_t)imm;
		break;
	case OPCODE_AND:
		gpr_val(cpu->regs, inst.rd) &= imm;
		break;
	case OPCODE_OR:
		gpr_val(cpu->regs, inst.rd) |= imm;
		break;
	case OPCODE_XOR:
		gpr_val(cpu->regs, inst.rd) ^= imm;
		break;
	case OPCODE_NOT:
		gpr_val(cpu->regs, inst.rd) = ~imm;
		break;
	case OPCODE_MOV:
		gpr_val(cpu->regs, inst.rd) = imm;
		break;
	}

	pc_move(cpu->pc, 1);
}

static void execute_shift(struct cpu_t *cpu, instruction_t inst)
{
	switch (inst.opcode) {
	case OPCODE_SHL:
		gpr_val(cpu->regs, inst.rd) <<= gpr_val(cpu->regs, inst.rs);
		break;
	case OPCODE_SHR:
		gpr_val(cpu->regs, inst.rd) >>= gpr_val(cpu->regs, inst.rs);
		break;
	}

	pc_move(cpu->pc, 1);
}

static void execute_shift_imm(struct cpu_t *cpu, instruction_t inst)
{
	pc_move(cpu->pc, 1);
	uint16_t imm;
	cpu_fetch(cpu, &imm);

	switch (inst.opcode) {
	case OPCODE_SHL:
		gpr_val(cpu->regs, inst.rd) <<= imm;
		break;
	case OPCODE_SHR:
		gpr_val(cpu->regs, inst.rd) >>= imm;
		break;
	}
	pc_move(cpu->pc, 1);
}

int cpu_execute(struct cpu_t *cpu)
{
	uint16_t inst_raw;
	int result = cpu_fetch(cpu, &inst_raw);

	if (result < 0)
		return result;

	instruction_t inst = { .opcode = (inst_raw & INST_OPCODE) >> 11,
			       .rd = (inst_raw & INST_RD) >> 7,
			       .flags = (inst_raw & INST_FLAGS) >> 4,
			       .rs = (inst_raw & INST_RS) };

	switch (inst.opcode) {
	case OPCODE_ADD:
	case OPCODE_SUB:
	case OPCODE_SHA:
	case OPCODE_AND:
	case OPCODE_OR:
	case OPCODE_XOR:
	case OPCODE_NOT:
	case OPCODE_MOV:
		if (inst.flags & IMM_FLAG)
			execute_general_imm(cpu, inst);
		else
			execute_general(cpu, inst);

		break;
	case OPCODE_SHL:
	case OPCODE_SHR:
		if (inst.flags & IMM_FLAG)
			execute_shift_imm(cpu, inst);
		else
			execute_shift(cpu, inst);

		break;
	default:
		return -EINVAL;
		break;
	}

	return 0;
}
