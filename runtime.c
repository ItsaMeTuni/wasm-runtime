#include "common.h"
#include "bytecode.h"

typedef struct {
    short type;
    union {
        unsigned int u32;
        unsigned long u64;
    } value;
} Value;

typedef struct {
    unsigned int arity;
    unsigned int offset;
} Label;

typedef struct {
    Value *locals;
    unsigned int locals_len;
    unsigned int function_arity;
} Frame;

typedef union {
    Value value;
    Label label;
    Frame frame;
} Item;

typedef struct {
    Item* stack;
    unsigned int stack_len;

    unsigned int next_instr_offset;

    Module module;
} Store;

void step(Store *store) {
    char instr = store->module.bytecode[store->next_instr_offset]; 
    store->next_instr_offset++;

    
}
