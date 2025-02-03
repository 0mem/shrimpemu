#ifndef CPU_H
#define CPU_H

#include <bus.h>

#include <stdint.h>

/* SHRIMP defines 16 general-purpose registers. */
#define NUM_REGS 16

/* General-purpose registers should be accessed only by the CPU instructions, so we are opaquing them. */
typedef struct {
	uint16_t *regs;
} gpr_t;

/* Initializer, Accessor Macros */
#define gpr(x) ((gpr_t){ (x) })
#define gpr_val(x, y) ((x).regs[y])
#define gpr_ptr(x) ((x).regs)

struct cpu_t {
	gpr_t regs; /* General-Purpose Registers */
	uint16_t flag_reg; /* Flags Register */
	uint16_t pc; /* Program Counter Register */
	struct bus_t *bus;
};

/*
	SHRIMP Instruction Structure
	---------------------------------------------------------------------------
	[ Opcode ] [ Destination ] [ Flags ] [ Source ] (+ [ Immediate Value ])

	Opcode: 		first 5 bits 	0b1111100000000000 = 0xf800
	Destination: 	next 4 bits		0b0000011110000000 = 0x780
	Flags:			next 3 bits		0b0000000001110000 = 0x70
	Source:			next 4 bits		0b0000000000001111 = 0xf

	If (imm?) flag is set (0x4) then the following word is the immediate value.
	---------------------------------------------------------------------------
 */
#define INST_OPCODE 0xf800
#define INST_RD 0x780
#define INST_FLAGS 0x70
#define INST_RS 0xf

/*
	SHRIMP Instruction Operations
	---------------------------------------------------------------------------
	
	---------------------------------------------------------------------------
 */
#define OPCODE_ADD 0
#define OPCODE_SUB 1
#define OPCODE_MUL 2
#define OPCODE_DIV 3
#define OPCODE_SHA 4
#define OPCODE_MOD 5
#define OPCODE_AND 6
#define OPCODE_OR 7
#define OPCODE_XOR 8
#define OPCODE_NOT 9
#define OPCODE_SHL 10
#define OPCODE_SHR 11
#define OPCODE_JMP 12
#define OPCODE_CALL OPCODE_JMP
#define OPCODE_RET 13
#define OPCODE_RETI OPCODE_RET
#define OPCODE_INT 14
#define OPCODE_MOV 15
#define OPCODE_LD 16
#define OPCODE_ST 17
#define OPCODE_LDFLG 18
#define OPCODE_STFLG 19

/*
	SHRIMP Instruction Flags
	---------------------------------------------------------------------------


	---------------------------------------------------------------------------
 */
#define IMM_FLAG 0x4

/*
	SHRIMP Flag Register
	---------------------------------------------------------------------------
	Negative Flag:
	Zero Flag:
	Positive Flag:
	Carryout Flag:
	Overflow Flag:
	---------------------------------------------------------------------------
 */

typedef struct {
	uint16_t opcode;
	uint16_t rd; /* Destination Register */
	uint16_t flags;
	uint16_t rs; /* Source Register */
} instruction_t;

struct cpu_t *cpu_alloc(void);
void cpu_free(struct cpu_t *cpu);

int cpu_execute(struct cpu_t *cpu);

#endif
