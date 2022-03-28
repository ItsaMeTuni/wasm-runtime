#include "bytecode.h"

#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include <utility>

#define SECTION_ID_EXPORTS 7
#define SECTION_ID_FUNCTIONS 3
#define SECTION_ID_CODE 10
#define SECTION_ID_TYPES 1

Bytecode::Bytecode(std::vector<char> bytecode) {
    this->bytecode = std::move(bytecode);
}

u32 Bytecode::read_u32(u32& offset) {
    u32 out = 0;
    short bytes_processed = 0;
    bool has_next_byte = true;

    while (has_next_byte) {
        char byte = bytecode[offset + bytes_processed];

        char zeroed_msb = (byte & 0b01111111);
        unsigned int padded = zeroed_msb << (bytes_processed * 7);
        out |= padded;

        has_next_byte = (byte & 0b10000000) != 0; 
        bytes_processed++;
    }

    offset += bytes_processed;

    return out;
}

std::string Bytecode::read_string(u32 &offset) {
    unsigned int len = read_u32(offset);

    printf("string len: %d\n", len);

    auto str = std::string(bytecode.begin() + offset, bytecode.begin() + offset + len);

    offset += len;

    return str;
}

unsigned long Bytecode::size() {
    return bytecode.size();
}

char Bytecode::at(unsigned int offset) {
    return bytecode[offset];
}

char Bytecode::read_char(u32& offset) {
    return at(offset++);
}

