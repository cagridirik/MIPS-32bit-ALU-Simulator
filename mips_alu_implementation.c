#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Function Prototypes...
uint8_t get_operation_code (uint32_t);
uint8_t get_source_register (uint32_t);
uint8_t get_second_source_register (uint32_t);
uint8_t get_destination_register (uint32_t);
uint8_t get_shift_amount (uint32_t);
uint8_t get_function (uint32_t);
void execute_r_type (uint32_t,  uint32_t[32]);
void addition(uint8_t, uint8_t, uint8_t, uint32_t[32]);
void subtraction(uint8_t, uint8_t, uint8_t, uint32_t[32]);
void logic_and(uint8_t, uint8_t, uint8_t, uint32_t[32]);
void logic_or(uint8_t, uint8_t, uint8_t, uint32_t[32]);
void shift_left(uint8_t, uint8_t, uint8_t, uint32_t[32]);
void shift_right(uint8_t, uint8_t, uint8_t, uint32_t[32]);
void set_less_than(uint8_t, uint8_t, uint8_t, uint32_t[32]);

int main() {
    // The register file containing 32 32-bit-wide registers can be simulated by an array of unsigned 32-bit integers.
    uint32_t register_file[32] = {0};
    for (int i = 1; i < 32; i++) {
        // Fill the registers 1-31 with numbers 0-100 because register 0 always has to be 0.
        uint32_t random_number = rand() % 100;

        // Make some of them negative.
        if (rand() % 2 == 0) random_number = (uint32_t)(-(int32_t)random_number);
        register_file[i] = random_number;
    }

    // Ask the user for a 32-bit hexadecimal instruction.
    uint32_t instruction;
    printf("Type a 32-bit hexadecimal instruction: ");
    scanf("%x", &instruction);
    execute_r_type(instruction, register_file);
}

// Remember the smallest unsigned integer is 8 bits wide in the stdint.h library.

uint8_t get_operation_code (uint32_t instruction) {
    // We need bits 31-26 for the opcode with leading 0s since it's stored in an 8-bit variable.
    // So, right shift the instruction 26 units and perform AND with 0011|1111, which is 3F in hexadecimal.
    uint8_t opcode = (instruction >> 26) & 0x3F;
    return opcode;
}

uint8_t get_source_register (uint32_t instruction) {
    // We need bits 25-21 for the first source register with leading 0s.
    // So, right shift the instruction 21 units and perform AND with 0001|1111, which is 1F in hexadecimal.
    uint8_t rs = (instruction >> 21) & 0x1F;
    return rs;
}

uint8_t get_second_source_register (uint32_t instruction) {
    // We need bits 20-16 for the second source register leading 0s.
    // So, right shift the instruction 16 units and perform AND with 0001|1111, which is 1F in hexadecimal.
    uint8_t rt = (instruction >> 16) & 0x1F;
    return rt;
}

uint8_t get_destination_register (uint32_t instruction) {
    // We need bits 15-11 for the destination register with leading 0s.
    // So, right shift the instruction 11 units and perform AND with 0001|1111, which is 1F in hexadecimal.
    uint8_t rd = (instruction >> 11) & 0x1F;
    return rd;
}

uint8_t get_shift_amount (uint32_t instruction) {
    // We need bits 10-6 for the destination register with leading 0s.
    // So, right shift the instruction 6 units and perform AND with 0001|1111, which is 1F in hexadecimal.
    uint8_t shamt = (instruction >> 6) & 0x1F;
    return shamt;
}

uint8_t get_function (uint32_t instruction) {
    // We need bits 5-0 for the destination register with leading 0s.
    // So, leave the instruction as it is and perform AND with 0011|1111, which is 1F in hexadecimal.
    uint8_t funct = instruction & 0x3F;
    return funct;
}

void execute_r_type (uint32_t instruction, uint32_t regFile[32]) {
    uint8_t opcode = get_operation_code(instruction);
    uint8_t rs = get_source_register(instruction);
    uint8_t rt = get_second_source_register(instruction);
    uint8_t rd = get_destination_register(instruction);
    uint8_t shamt = get_shift_amount(instruction);
    uint8_t funct = get_function(instruction);

    // Check if the hex code (instruction) is R-type or I-type/J-type. This file will focus on R-type instructions.
    if (opcode == 0x00) {
        printf("An R-Type instruction is detected. Checking the function now...\n");
        
        // The funct of add is 0x20/
        if (funct == 0x20) addition(rs, rt, rd, regFile);

        // The funct of sub is 0x22.
        else if (funct == 0x22) subtraction(rs, rt, rd, regFile);

        // The funct of and is 0x24.
        else if (funct == 0x24) logic_and(rs, rt, rd, regFile);

        // The funct of or is 0x25.
        else if (funct == 0x25) logic_or(rs, rt, rd, regFile);

        // The funct of sll (shift left logic) is 0x00.
        else if (funct == 0x00) shift_left(rt, rd, shamt, regFile);

        // The funct of srl (shift right logic) is 0x02.
        else if (funct == 0x02) shift_right(rt, rd, shamt, regFile);

        // The funct of slt (set less than) is 0x2A.
        else if (funct == 0x2A) set_less_than(rs, rt, rd, regFile);

        else printf("The function is beyond the scope of this project.");
    }
    else printf("This is not an R-Type instruction and is out of the scope of this project.");
}

void addition(uint8_t rs, uint8_t rt, uint8_t rd, uint32_t regFile[32]) {
    int32_t signed_rs = (int32_t) regFile[rs];
    int32_t signed_rt = (int32_t) regFile[rt];
    int32_t signed_rd = signed_rs + signed_rt;
        
    // Also check for a sign overflow (adding two + numbers and getting a - number or vice versa)
    if ((signed_rs > 0 && signed_rt > 0 && signed_rd < 0) || (signed_rs < 0 && signed_rt < 0 && signed_rd > 0)) {
        printf("WARNING, SIGN OVERFLOW DETECTED!");
        exit(1); // Stops the code from execution.
    }
    regFile[rd] = (uint32_t) signed_rd;

    // MIPS Assembly version of this function is "add $rd, $rs, $rt"
    printf("OPERATION: add $%d, $%d, $%d\n", rd, rs, rt);
    printf("VALUES: Adding %d and %d yields %d.\n", signed_rs, signed_rt, signed_rd);
}

void subtraction(uint8_t rs, uint8_t rt, uint8_t rd, uint32_t regFile[32]) {
    int32_t signed_rs = (int32_t) regFile[rs];
    int32_t signed_rt = (int32_t) regFile[rt];
    int32_t signed_rd = signed_rs - signed_rt;
        
    // Also check for a sign overflow (adding two + numbers and getting a - number or vice versa)
    if ((signed_rs >= 0 && signed_rt < 0 && signed_rd < 0) || (signed_rs < 0 && signed_rt >= 0 && signed_rd > 0)) {
        printf("WARNING, SIGN OVERFLOW DETECTED!");
        exit(1); // Stops the code from execution.
    }
    regFile[rd] = (uint32_t) signed_rd;

    // MIPS Assembly version of this function is "sub $rd, $rs, $rt"
    printf("OPERATION: sub $%d, $%d, $%d\n", rd, rs, rt);
    printf("VALUES: Subtracting %d from %d yields %d.\n", signed_rt, signed_rs, signed_rd);
}

// & performs bitwise AND, nothing else is required.
void logic_and (uint8_t rs, uint8_t rt, uint8_t rd, uint32_t regFile[32]) {
    regFile[rd] = regFile[rs] & regFile[rt];

    // MIPS Assembly version of this function is "and rd, rs, rt"
    printf("OPERATION: and $%d, $%d, $%d\n", rd, rs, rt);
    printf("VALUES: %d AND %d yields %d.\n", regFile[rs], regFile[rt], regFile[rd]);
}

// | performs bitwise OR, nothing else is required.
void logic_or (uint8_t rs, uint8_t rt, uint8_t rd, uint32_t regFile[32]) {
    regFile[rd] = regFile[rs] | regFile[rt];

    // MIPS Assembly version of this function is "or rd, rs, rt"
    printf("OPERATION: or $%d, $%d, $%d\n", rd, rs, rt);
    printf("VALUES: %d OR %d yields %d.\n", regFile[rs], regFile[rt], regFile[rd]);
}

// Shift functions does not use rs.
void shift_left (uint8_t rt, uint8_t rd, uint8_t shamt, uint32_t regFile[32]) {
    regFile[rd] = regFile[rt] << shamt;

    // MIPS Assembly version of this function is "sll rd, rt, shamt"
    printf("OPERATION: sll $%d, $%d, $%d\n", rd, rt, shamt);
    printf("VALUES: Shifting %d %d units to the left yields %d.\n", regFile[rt], shamt, regFile[rd]);
}

void shift_right (uint8_t rt, uint8_t rd, uint8_t shamt, uint32_t regFile[32]) {
    regFile[rd] = regFile[rt] >> shamt;

    // MIPS Assembly version of this function is "and rd, rt, shamt"
    printf("OPERATION: srl $%d, $%d, $%d\n", rd, rt, shamt);
    printf("VALUES: Shifting %d %d units to the right yields %d.\n", regFile[rt], shamt, regFile[rd]);
}

// If rs < rt, it sets rd to 1, and if rs >= rt, it sets rd to 0.
void set_less_than(uint8_t rs, uint8_t rt, uint8_t rd, uint32_t regFile[32]) {
    int32_t signed_rs = (int32_t) regFile[rs];
    int32_t signed_rt = (int32_t) regFile[rt];
    if (signed_rs < signed_rt) regFile[rd] = 1;
    else regFile[rd] = 0;

    // MIPS Assembly version of this function is "slt rd, rs, rt"
    printf("OPERATION: slt $%d, $%d, $%d\n", rd, rs, rt);
    printf("VALUES: Set %d less than %d yields %d.\n", rs, rt, regFile[rd]);
}
