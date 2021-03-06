#ifndef _RUNTIME_H
#define _RUNTIME_H

#include <memory>
#include <stack>
#include <variant>

#include "bytecode.h"
#include "Module.h"

#define INSTRUCTION_NOP 0x0
#define INSTRUCTION_CONST_I32 0x41
#define INSTRUCTION_CONST_I64 0x42
#define INSTRUCTION_LOCAL_GET 0x20
#define INSTRUCTION_I32_ADD 0x6A
#define INSTRUCTION_CALL 0x10
#define INSTRUCTION_END 0x0b

#define ITEM_TYPE_VALUE 0
#define ITEM_TYPE_LABEL 1
#define ITEM_TYPE_FRAME 2

#define VALUE_TYPE_I32 0
#define VALUE_TYPE_I64 1

typedef std::variant<u32, u64> Value;
//typedef struct {
//    short type;
//    union {
//        unsigned int i32;
//        unsigned long i64;
//    } value;
//} Value;

typedef struct {
    unsigned int arity;
    unsigned int offset;
} Label;

typedef struct {
    std::vector<Value> locals;
    unsigned int function_arity;
    unsigned int next_instr_offset;
} Frame;

typedef std::variant<Value, Label, Frame> Item;

//typedef struct {
//    char type;
//    union {
//        Value value;
//        Label label;
//        Frame frame;
//    } item;
//} Item;

class Store {
    std::vector<Item> stack;

    bool has_frame;

    // Only valid if has_frame is true
    u32 current_frame_item_idx;

    std::shared_ptr<Module> module;

public:

    Store(std::shared_ptr<Module> module) : module(module) {};

    void stack_push(Item item);
    Item stack_pop();
    void step();
    void invoke(unsigned int function_idx);
    void update_current_frame_data();
    void clear_stack();
    Frame &get_current_frame();
    void print_stack();
    static void print_item(Item &item);
};

void step(Store *store);
Store make_store(Module *module);
void invoke(Store *store, unsigned int function_idx);
void print_stack(Store *Store);

#endif
