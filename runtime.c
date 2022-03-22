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

Frame* get_current_frame(Store *store) {
    for(unsigned int i = 0; i < store->stack_len; i++) {
        Item *item = &store->stack[i];

        if(item->type == ITEM_TYPE_FRAME) {
            return &item->item.frame;
        }
    }

    // raise error

    return NULL;
}

void step(Store *store) {
    char instr = store->module->bytecode[store->next_instr_offset]; 
    store->next_instr_offset++;

    char *bytecode = store->module->bytecode;

    printf("processing instruction 0x%02x\n", instr);

    if(instr == INSTRUCTION_NOP) {
        return;

    } else if (instr == INSTRUCTION_CONST_I32) {
        // The const value is a u32 following the const.i32 instruction
        unsigned int u32_value = 0;
        store->next_instr_offset += read_u32(bytecode, store->next_instr_offset, &u32_value);
        
        // Push the u32 to the stack
        stack_push(store, (Item) { .type = ITEM_TYPE_VALUE, .item = (Value) { .type = VALUE_TYPE_I32, .value = u32_value } });

    } else if (instr == INSTRUCTION_LOCAL_GET) {
        // The local's index is a u32 following the local.get instruction
        unsigned int local_idx = 0;
        store->next_instr_offset += read_u32(bytecode, store->next_instr_offset, &local_idx);

        // Get the local from the current frame by it's index
        Frame *current_frame = get_current_frame(store);
        Value *local = &current_frame->locals[local_idx];

        // push local to stack
        stack_push(store, (Item) { .type = ITEM_TYPE_VALUE, .item = { .value = *local } });
    } else if (instr == INSTRUCTION_I32_ADD) {
        unsigned int a = stack_pop(store).item.value.value.i32;
        unsigned int b = stack_pop(store).item.value.value.i32;

        unsigned int result = a + b;
        Item result_item = (Item) { .type = ITEM_TYPE_VALUE, .item = (Value) { .type = VALUE_TYPE_I32, .value = { .i32 = result } } };

        stack_push(store, result_item);
    }
}

Store make_store(Module *module) {
    return (Store) {
        .stack = malloc(sizeof(Module) * 1024),
        .stack_len = 0,
        .next_instr_offset = -1,
        .module = module
    };
}

void invoke(Store *store, char *export_name) {
    for(unsigned int export_idx = 0; export_idx < store->module->exports_len; export_idx++) {
        Export *export = &store->module->exports[export_idx];
        if(strcmp(export->name, export_name) == 0) {
            store->next_instr_offset = store->module->functions[export->exportee_idx].offset;
        }
    }
}

void print_item(Item *item) {
    if(item->type == ITEM_TYPE_FRAME) {
        printf("Frame\n");
    } else if (item->type == ITEM_TYPE_LABEL) {
        printf("Label\n");
    } else if (item->type == ITEM_TYPE_VALUE) {
        printf("Value\n");
    }
}

void print_stack(Store *store) {
    printf("Stack: (%d)\n", store->stack_len);
    for(int i = store->stack_len - 1; i >= 0; i--) {
        Item *item = &store->stack[i];
        print_item(item);
    }
}
