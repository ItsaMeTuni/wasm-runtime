#include "runtime.h"
#include "bytecode.h"
#include <cstdlib>
#include <cstdio>

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

    char instr = module->bytecode->read_char(offset);

    printf("processing instruction 0x%02x (offset 0x%02x)\n", instr, offset - 1);

    if(instr == INSTRUCTION_NOP) {
        return;

    } else if (instr == INSTRUCTION_CONST_I32) {
        // The const value is a u32 following the const.i32 instruction
        u32 u32_value = module->bytecode->read_u32(offset);
        
        // Push the u32 to the stack
        stack_push(Item(Value(u32_value)));
    } else if (instr == INSTRUCTION_LOCAL_GET) {
        // The local's index is a u32 following the local.get instruction
        unsigned int local_idx = module->bytecode->read_u32(offset);

        // Get the local from the current frame by it's index
        Value *local = &current_frame->locals[local_idx];

        // push local to stack
        stack_push(Item(*local));
    } else if (instr == INSTRUCTION_I32_ADD) {
        unsigned int a = std::get<u32>(std::get<Value>(stack_pop()));
        unsigned int b = std::get<u32>(std::get<Value>(stack_pop()));

        unsigned int result = a + b;
        Item result_item = Item(Value(result));

        stack_push(result_item);
    } else if (instr == INSTRUCTION_CALL) {
        unsigned int function_idx = module->bytecode->read_u32(offset);

        invoke(function_idx);
    }
}

void Store::invoke(unsigned int function_idx) {
    Function &fn = module->functions.at(function_idx);
    Frame frame = { .next_instr_offset = fn.offset };
    Item item = Item(frame);

    Type &fn_type = module->types.at(fn.type_idx);
    
    // TODO: add locals to locals array
    for(unsigned int param_idx = 0; param_idx < fn_type.params.size(); param_idx++) {
        auto param = std::get<Value>(stack_pop());
        frame.locals.push_back(param);

    }

    stack_push(item);

    // This is BAD, if the vector reallocates we have an invalid pointer
    // TODO: deal with this
    current_frame = &std::get<Frame>(stack.back());
    std::deque<Item> a;
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
void Store::print_item(Item &item) {
    std::visit(overloaded {
            [](Frame frame) { printf("Frame\n"); },
            [](Label label) { printf("Label\n"); },
            [](Value value) { printf("Value %d\n", std::get<u32>(value)); }
    }, item);
}

void Store::print_stack() {
    printf("Stack: (%ld)\n", stack.size());
    for(long i = stack.size() - 1; i >= 0; i--) {
        printf("printing stack item %ld, stack size %ld\n", i, stack.size());
        Store::print_item(stack[i]);
    }
}
