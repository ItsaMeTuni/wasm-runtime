#include "runtime.h"
#include "bytecode.h"
#include "stdlib.h"
#include "stdio.h"
#include <string.h>

Item Store::stack_pop() {
    Item item = stack.back();
    stack.pop_back();
    return item;
}

void Store::stack_push(Item item) {
    stack.push_back(item);
}

void Store::step() {
    unsigned int &offset = current_frame->next_instr_offset;

    char instr = module->bytecode->at(offset);
    offset++;

    printf("processing instruction 0x%02x (offset 0x%02x)\n", instr, offset);

    if(instr == INSTRUCTION_NOP) {
        return;

    } else if (instr == INSTRUCTION_CONST_I32) {
        // The const value is a u32 following the const.i32 instruction
        unsigned int u32_value = module->bytecode->read_u32(offset);
        
        // Push the u32 to the stack
        stack_push((Item) { .type = ITEM_TYPE_VALUE, .item = (Value) { .type = VALUE_TYPE_I32, .value = u32_value } });

    } else if (instr == INSTRUCTION_LOCAL_GET) {
        // The local's index is a u32 following the local.get instruction
            unsigned int local_idx = module->bytecode->read_u32(offset);

        // Get the local from the current frame by it's index
        Value *local = &current_frame->locals[local_idx];

        // push local to stack
        stack_push((Item) { .type = ITEM_TYPE_VALUE, .item = { .value = *local } });
    } else if (instr == INSTRUCTION_I32_ADD) {
        unsigned int a = stack_pop().item.value.value.i32;
        unsigned int b = stack_pop().item.value.value.i32;

        unsigned int result = a + b;
        Item result_item = (Item) { .type = ITEM_TYPE_VALUE, .item = (Value) { .type = VALUE_TYPE_I32, .value = { .i32 = result } } };

        stack_push(result_item);
    } else if (instr == INSTRUCTION_CALL) {
        unsigned int function_idx = module->bytecode->read_u32(offset);

        invoke(function_idx);
    }
}

void Store::invoke(unsigned int function_idx) {
    Function *fn = &module->functions[function_idx];
    Frame frame = (Frame) { .next_instr_offset = fn->offset };
    Item item = (Item) { .type = ITEM_TYPE_FRAME, .item = { .frame = frame }};

    Type *fn_type = &module->types[fn->type_idx];


    // TODO: allocate space for locals
    frame.locals = (Value*) malloc(sizeof(Value) * fn_type->params.size());
    printf("hat\n");

    for(unsigned int param_idx = 0; param_idx < fn_type->params.size(); param_idx++) {
        printf("hat %d %ld\n", param_idx, fn_type->params.size());
        frame.locals[param_idx] = stack_pop().item.value;
    }

    stack_push(item);

    // This is BAD, if the vector reallocates we have an invalid pointer
    current_frame = &stack.back().item.frame;
    std::deque<Item> a;
}

void Store::print_item(Item *item) {
    if(item->type == ITEM_TYPE_FRAME) {
        printf("Frame\n");
    } else if (item->type == ITEM_TYPE_LABEL) {
        printf("Label\n");
    } else if (item->type == ITEM_TYPE_VALUE) {
        printf("Value %d\n", item->item.value.value.i32);
    }
}

void Store::print_stack(Store *store) {
    printf("Stack: (%ld)\n", stack.size());
    for(unsigned long i = stack.size(); i >= 0; i--) {
        Store::print_item(&stack[i]);
    }
}
