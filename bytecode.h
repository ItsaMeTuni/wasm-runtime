#ifndef _BYTECODE_H
#define _BYTECODE_H

#include <vector>
#include <string>

#define TYPE_NUMBER_I32 0x7f
#define TYPE_NUMBER_I64 0x7E
#define TYPE_NUMBER_F32 0x7D
#define TYPE_NUMBER_F64 0x7C
#define TYPE_FUNCTION 0x60

typedef unsigned int u32;



class Bytecode {
    std::vector<char> bytecode;

public:
    Bytecode(std::vector<char> bytecode);

    char read_char(u32& offset);
    u32 read_u32(u32& offset);
    std::string read_string(u32& offset);
    template<typename T> std::vector<T> read_vector(T (Bytecode::* reader)(u32& offset), u32& offset);

    unsigned long size();
    char at(unsigned int offset);
};

Module* read_module(char *bytecode, unsigned int bytecode_len);
unsigned int read_u32(char *bytecode, unsigned int offset, unsigned int* out);

#endif
