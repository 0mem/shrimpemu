#ifndef CPU_H
#define CPU_H

#include <bus.h>

#include <stdint.h>

/* SHRIMP defines 16 general-purpose registers. */
#define NUM_REGS 16

/* General-purpose registers should be accessed only by the CPU instructions, so we are opaquing them. */
/*
	SHRIMP General-purpose Registers Structure
	---------------------------------------------------------------------------
	r0 (rz): Zero Register (Zero register always contains zero and discards any write to the register)
	r1-r14 : Free-to-use
	r15 (sp): Stack Pointer (It contains address of the top of the stack)
	---------------------------------------------------------------------------
 */
typedef struct {
	uint16_t *regs;
} gpr_t;

/* Initializer, Accessor Macros */
#define gpr(x) ((gpr_t){ (x) })
#define gpr_val(x, y) ((x).regs[y])
#define gpr_ptr(x) ((x).regs)

/* flags register should be accessed only by the CPU instructions, so we are opaquing them. */
typedef struct {
	uint16_t flag_reg
} freg_t;

/* Initializer, Accessor Macros */
#define freg(x) ((freg_t){ (x) })
#define freg_val(x) ((x).flag_reg)

/* program counter register should be accessed only by the CPU instructions, so we are opaquing them. */
typedef struct {
	uint16_t pc;
} pc_t;

/* Initializer, Accessor Macros */
#define pc(x) ((pc_t){ (x) })
#define pc_val(x) ((x).pc)
#define pc_move(x, y) ((x).pc += y)

struct cpu_t {
	gpr_t regs; /* General-Purpose Registers */
	freg_t flag_reg; /* Flags Register */
	pc_t pc; /* Program Counter Register */
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
	OP1 : destination register | OP2 : source register, or an immediate.

	Operations List
	
	add: Add the op2 to op1, and store the result in rd
	sub: Subtract op2 from op1 and store the result in rd
	mul: Store the product of op1 and op2 in rd, with signed and unsigned varieties
	div: Divide op1 by op2 and store the result in rd
	sha: Arithmetic right shift of op1 by op2, storing the result in rd
	mod: Find the remainder of op1 divided by op2, storing the result in rd
	and: Bitwise AND of op1 and op2, storing the result in rd
	or : Bitwise OR of op1 and op2, storing the result in rd
	xor: Bitwise XOR of op1 and op2, storing the result in rd
	not: Bitwise negation of op2, storing the result in rd
	shl: Left shift of op1 by op2, storing the result in rd
	rol: Rotate op1 left by op2, storing the result in rd
	shr: Right shift of op1 by op2, storing the result in rd
	ror: Rotate op1 right by op2, storing the result in rd
	jmp: When conditions are satisfied, jump to op1 (sets PC to op1)
	call: When conditions are satisfied, push PC onto the stack, and jump to op1
	ret: Pop PC from the stack (used to return from call)
	reti: Restore all registers from the stack, then pop PC off (used to return from an interrupt)
	int: Push PC onto the stack, save all registers onto the stack, then jump to the interrupt op1
	mov: Copy the value of op2 into rd
	ld: Load the value at memory address op2 into rd
	st: Store the value of rs (which is op1) into memory address op2
	ldflg: Load the flags register into rd
	stflg: Set the flags register to the value rs
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
	IMM: Whether to use next word as immediate value of the instruction.
	WRD: Whether the instruction operates on a word.
	SGN: Whether the instruction is signed operation.
	ROT: Whether the shift instruction rotates the word.
	---------------------------------------------------------------------------
 */
#define IMM_FLAG 0x4
#define WRD_FLAG 0x2
#define SGN_FLAG 0x1
#define ROT_FLAG 0x1

/*
	SHRIMP Flag Register
	---------------------------------------------------------------------------
	  Flags can be appended to some special opcodes (call, jmp) and the flag is
	stored in the flags register in CPU.

	Negative Flag: Branch if this flag(NF) is set.
	Zero Flag: Branch if this flag(ZF) is set.
	Positive Flag: Branch if this flag(PF) is set.
	Carryout Flag: Branch if this flag(CF) is set.
	Overflow Flag: Branch if this flag(OF) is set.

	JMP, CALL instructions has special forms, as
	[ Opcode ] [ Destination ] [ IMM? ] [ NF? ] [ ZF? ] [ PF? ] [ CF? ] [ OF? ] [ 0? 1? ]
	Each flags takes signe bit.
	---------------------------------------------------------------------------
 */
#define FREG_NEGATIVE 0x10
#define FREG_ZERO 0x8
#define FREG_POSITIVE 0x4
#define FREG_CARRYOUT 0x2
#define FREG_OVERFLOW 0x1

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
