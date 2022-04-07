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
    unsigned int &offset = get_current_frame().next_instr_offset;

    char instr = module->bytecode->read_char(offset);

    printf("processing instruction 0x%02x (offset 0x%02x)\n", instr, offset - 1);
    printf("current frame: local count: %ld\n", get_current_frame().locals.size());

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
        Value local = get_current_frame().locals.at(local_idx);

        // push local to stack
        stack_push(Item(local));
    } else if (instr == INSTRUCTION_I32_ADD) {
        unsigned int a = std::get<u32>(std::get<Value>(stack_pop()));
        unsigned int b = std::get<u32>(std::get<Value>(stack_pop()));

        unsigned int result = a + b;
        Item result_item = Item(Value(result));

        stack_push(result_item);
    } else if (instr == INSTRUCTION_CALL) {
        unsigned int function_idx = module->bytecode->read_u32(offset);

        invoke(function_idx);
    } else if (instr == INSTRUCTION_END) {
        auto arity = get_current_frame().function_arity;

        // Move all the values on top of the stack to before the frame
        for(auto i = 0; i < arity; i++) {
            auto itPos = stack.begin() + current_frame_item_idx - 1;
            stack.insert(itPos, stack_pop());
        }

        // pop the frame
        stack_pop();

        printf("stack size is %ld\n", stack.size());
        for(long i = stack.size(); i >= 0; i--) {
            printf("bruh %d\n", i);
            if (std::holds_alternative<Frame>(stack.at(i))) {
                printf("new current frame is at idx %ld\n", i);
                current_frame_item_idx = i;
                break;
            }
        }
    }
}

void Store::invoke(unsigned int function_idx) {
    Function &fn = module->functions.at(function_idx);
    Frame frame = { .next_instr_offset = fn.offset };

    Type &fn_type = module->types.at(fn.type_idx);
    
    // TODO: add locals to locals array
    for(unsigned int param_idx = 0; param_idx < fn_type.params.size(); param_idx++) {
        auto param = std::get<Value>(stack_pop());
        frame.locals.push_back(param);
    }

    Item item = Item(frame);
    stack_push(item);

    // This is BAD, if the vector reallocates we have an invalid pointer
    // TODO: deal with this
    current_frame_item_idx = stack.size() - 1;
    std::deque<Item> a;
}

Frame &Store::get_current_frame() {
    return std::get<Frame>(stack.at(current_frame_item_idx));
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
        Store::print_item(stack[i]);
    }
}
