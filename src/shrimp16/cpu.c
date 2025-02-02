#include <cpu.h>

uint16_t register_read(registers_t *registers, size_t index) {
    return registers->registers[index];
}

void register_write(registers_t *registers, size_t index, uint16_t value) {
    registers->registers[index] = value;
}

cpu_t *cpu_new(void) {
    cpu_t *cpu = (cpu_t *)malloc(sizeof(cpu_t));
    memset(cpu->registers.registers, 0, NUM_REGISTERS);
    cpu->flags_register = 0;
    cpu->program_counter = 0;
    cpu->bus = bus_new();
    return cpu;
}

void cpu_free(cpu_t *cpu) {
    bus_free(cpu->bus);
    free(cpu);
}

uint16_t cpu_fetch(cpu_t *cpu) {
    uint16_t present_program_counter = cpu->program_counter;

    return bus_read(cpu->bus, present_program_counter);
}

void execute(cpu_t *cpu) {
    uint16_t instruction_raw = cpu_fetch(cpu);
    instruction_t instruction;
    instruction_new(&instruction, instruction_raw);

    printf("0x%04x\n", instruction_raw);
    printf("Opcode: %d | Dest: %d | Flags: %d | Src: %d\n", instruction.opcode, instruction.destination, instruction.flags, instruction.source);
    
    switch (instruction.opcode) {
    case ADD:
        if (instruction.flags & 0x4) {
            execute_imm(cpu, instruction);
        } else {
            execute_general(cpu, instruction);
        }
        cpu->program_counter += 1;
    
    default:
        break;
    }
}

void execute_general(cpu_t *cpu, instruction_t instruction) {
}

void execute_imm(cpu_t *cpu, instruction_t instruction) {
    cpu->program_counter += 1;
    uint16_t immediate = cpu_fetch(cpu);

    switch (instruction.opcode) {
    case ADD:
        register_write(&cpu->registers, instruction.destination
            , register_read(&cpu->registers, instruction.destination) + immediate);
        break;
    
    default:
        break;
    }
}
