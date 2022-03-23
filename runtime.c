#include "runtime.h"
#include "common.h"
#include "bytecode.h"
#include "stdlib.h"
#include "stdio.h"
#include <string.h>

void stack_push(Store *store, Item item) {
    store->stack[store->stack_len] = item;
    store->stack_len++;
}

Item stack_pop(Store *store) {
    store->stack_len--;
    return store->stack[store->stack_len];
}

void step(Store *store) {
    unsigned int *offset = &store->current_frame->next_instr_offset;

    char instr = store->module->bytecode[*offset];
    (*offset)++;

    char *bytecode = store->module->bytecode;

    printf("processing instruction 0x%02x (offset 0x%02x)\n", instr, *offset);

    if(instr == INSTRUCTION_NOP) {
        return;

    } else if (instr == INSTRUCTION_CONST_I32) {
        // The const value is a u32 following the const.i32 instruction
        unsigned int u32_value = 0;
        *offset += read_u32(bytecode, *offset, &u32_value);
        
        // Push the u32 to the stack
        stack_push(store, (Item) { .type = ITEM_TYPE_VALUE, .item = (Value) { .type = VALUE_TYPE_I32, .value = u32_value } });

    } else if (instr == INSTRUCTION_LOCAL_GET) {
        // The local's index is a u32 following the local.get instruction
        unsigned int local_idx = 0;
        *offset += read_u32(bytecode, *offset, &local_idx);

        // Get the local from the current frame by it's index
        Value *local = &store->current_frame->locals[local_idx];

        // push local to stack
        stack_push(store, (Item) { .type = ITEM_TYPE_VALUE, .item = { .value = *local } });
    } else if (instr == INSTRUCTION_I32_ADD) {
        unsigned int a = stack_pop(store).item.value.value.i32;
        unsigned int b = stack_pop(store).item.value.value.i32;

        unsigned int result = a + b;
        Item result_item = (Item) { .type = ITEM_TYPE_VALUE, .item = (Value) { .type = VALUE_TYPE_I32, .value = { .i32 = result } } };

        stack_push(store, result_item);
    } else if (instr == INSTRUCTION_CALL) {
        unsigned int function_idx = 0;
        *offset += read_u32(bytecode, *offset, &function_idx);

        invoke(store, function_idx);
    }
}

Store make_store(Module *module) {
    return (Store) {
        .stack = malloc(sizeof(Module) * 1024),
        .stack_len = 0,
        .module = module
    };
}

void invoke(Store *store, unsigned int function_idx) {
    Function *fn = &store->module->functions[function_idx];
    Frame frame = (Frame) { .next_instr_offset = fn->offset };
    Item item = (Item) { .type = ITEM_TYPE_FRAME, .item = { .frame = frame }};

    Type *fn_type = &store->module->types[fn->type_idx];


    // TODO: allocate space for locals
    frame.locals = malloc(sizeof(Value) * fn_type->params_len);
    printf("hat\n");

    for(unsigned int param_idx = 0; param_idx < fn_type->params_len; param_idx++) {
        printf("hat %d %d\n", param_idx, fn_type->params_len);
        frame.locals[param_idx] = stack_pop(store).item.value;
    }

    stack_push(store, item);

    store->current_frame = &store->stack[store->stack_len - 1].item.frame;
}

void print_item(Item *item) {
    if(item->type == ITEM_TYPE_FRAME) {
        printf("Frame\n");
    } else if (item->type == ITEM_TYPE_LABEL) {
        printf("Label\n");
    } else if (item->type == ITEM_TYPE_VALUE) {
        printf("Value %d\n", item->item.value.value.i32);
    }
}

void print_stack(Store *store) {
    printf("Stack: (%d)\n", store->stack_len);
    for(int i = store->stack_len - 1; i >= 0; i--) {
        Item *item = &store->stack[i];
        print_item(item);
    }
}
