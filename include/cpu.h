#ifndef CPU_H
#define CPU_H

#include <bus.h>

#include <stdint.h>

#define NUM_REGS 16

typedef struct {
	uint16_t *regs;
} gpr_t;

#define gpr(x) ((gpr_t){ (x) })
#define gpr_val(x, y) ((x).regs[y])
#define gpr_ptr(x) ((x).regs)

struct cpu_t {
	gpr_t regs;
	uint16_t flag_reg;
	uint16_t pc;
	struct bus_t *bus;
};

#define INST_OPCODE 0xf800
#define INST_RD 0x780
#define INST_FLAGS 0x70
#define INST_RS 0xf

typedef struct {
	uint16_t opcode;
	uint16_t rd;
	uint16_t flags;
	uint16_t rs;
} instruction_t;

struct cpu_t *cpu_alloc(void);
void cpu_free(struct cpu_t *cpu);

int cpu_execute(struct cpu_t *cpu);

#endif
