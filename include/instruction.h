#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

#define INSTRUCTION_OPCODE 0xf800
#define INSTRUCTION_DESTINATION 0x780
#define INSTRUCTION_FLAGS 0x70
#define INSTRUCTION_SOURCE 0xf


typedef struct {
    uint16_t opcode;
    uint16_t destination;
    uint16_t flags;
    uint16_t source;
} instruction_t;

void instruction_new(instruction_t *instruction, uint16_t instruction_raw);

typedef enum {
    ADD = 0x00,
    SUB = 0x01,
    MUL = 0x02,
    DIV = 0x03,
    SHA = 0x04,
    MOD = 0x05,
    AND = 0x06,
    OR = 0x07,
    XOR = 0x08,
    NOT = 0x09,
    SHL = 0x0a,
    ROL = 0x0a,
    SHR = 0x0b,
    ROR = 0x0b,
    JMP = 0x0c,
    CALL = 0x0c,
    RET = 0x0d,
    RETI = 0x0d,
    INT = 0x0e,
    MOV = 0x0f,
    LD = 0x10,
    ST = 0x11,
    LDFLG = 0x12,
    STFLG = 0x13,
} opcode_t;

#endif
