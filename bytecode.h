#ifndef _BYTECODE_H
#define _BYTECODE_H

#define TYPE_NUMBER_I32 0x7f
#define TYPE_NUMBER_I64 0x7E
#define TYPE_NUMBER_F32 0x7D
#define TYPE_NUMBER_F64 0x7C
#define TYPE_FUNCTION 0x60

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
    char *params;
    unsigned int params_len;

    char *results;
    unsigned int results_len;
} Type;

typedef struct {
    Function *functions;
    unsigned int functions_len;

    Export *exports;
    unsigned int exports_len;

    Type *types;
    unsigned int types_len;

    char *bytecode;
    unsigned int bytecode_len;
} Module;

Module* read_module(char *bytecode, unsigned int bytecode_len);
unsigned int read_u32(char *bytecode, unsigned int offset, unsigned int* out);

#endif
