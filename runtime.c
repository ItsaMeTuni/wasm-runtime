#include "common.h"
#include "bytecode.h"

#define INSTRUCTION_NOP 0x0
#define INSTRUCTION_CONST_I32 0x41
#define INSTRUCTION_CONST_I64 0x42

#define ITEM_TYPE_VALUE 0
#define ITEM_TYPE_LABEL 1
#define ITEM_TYPE_FRAME 2

#define VALUE_TYPE_I32 0
#define VALUE_TYPE_I64 1

typedef struct {
    short type;
    union {
        unsigned int i32;
        unsigned long i64;
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

typedef struct {
    char type;
    union {
        Value value;
        Label label;
        Frame frame;
    } item;
} Item;

typedef struct {
    Item* stack;
    unsigned int stack_len;

    unsigned int next_instr_offset;

    Module module;
} Store;

void stack_push(Store *store, Item item) {
    store->stack[store->stack_len] = item;
    store->stack_len++;
}

void step(Store *store) {
    char instr = store->module.bytecode[store->next_instr_offset]; 
    store->next_instr_offset++;

    char *bytecode = store->module.bytecode;

    if(instr == INSTRUCTION_NOP) {
        return;
    } else if (instr == INSTRUCTION_CONST_I32) {
        unsigned int u32_value = 0;
        store->next_instr_offset += read_u32(bytecode, store->next_instr_offset, &u32_value);
        stack_push(store, (Item) { .type = ITEM_TYPE_VALUE, .item = (Value){ .type = VALUE_TYPE_I32, .value = u32_value } });
    }
}
