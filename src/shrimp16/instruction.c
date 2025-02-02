#include <instruction.h>

void instruction_new(instruction_t *instruction, uint16_t instruction_raw) {
    instruction->opcode = (instruction_raw & INSTRUCTION_OPCODE) >> 11;
    instruction->destination = (instruction_raw & INSTRUCTION_DESTINATION) >> 7;
    instruction->flags = (instruction_raw & INSTRUCTION_FLAGS) >> 4;
    instruction->source = (instruction_raw & INSTRUCTION_SOURCE);
}
