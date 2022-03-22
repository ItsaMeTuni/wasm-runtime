#ifndef _BYTECODE_H
#define _BYTECODE_H
typedef struct {
    char id;
    unsigned int offset;
    unsigned int length;
} Section;

typedef struct {
    unsigned int offset;
    unsigned int type_idx;
} Function;

typedef struct {
    char* name;
    // Type of the export
    char type;
    // Idx of function, table, mem or global based on type
    unsigned int exportee_idx;
} Export;

typedef struct {
    Function* functions;
    unsigned int functions_len;

    Export* exports;
    unsigned int exports_len;

    char *bytecode;
    unsigned int bytecode_len;
} Module;

Module* read_module(char *bytecode, unsigned int bytecode_len);
unsigned int read_u32(char *bytecode, unsigned int offset, unsigned int* out);

#endif
