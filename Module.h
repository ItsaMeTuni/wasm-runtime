//
// Created by lucas on 3/23/22.
//

#ifndef WASM_RUNTIME_MODULE_H
#define WASM_RUNTIME_MODULE_H

#include <vector>
#include <memory>
#include "bytecode.h"

typedef struct {
    char id;
    u32 offset;
    u32 length;
} Section;

typedef struct {
    u32 offset;
    u32 type_idx;
} Function;

struct Export {
    std::string name;
    // Type of the export
    char type;
    // Idx of function, table, mem or global based on type
    unsigned int exportee_idx;
};

struct Type {
    std::vector<char> params;
    std::vector<char> results;
};

struct Module {
    std::vector<Function> functions;
    std::vector<Export> exports;
    std::vector<Type> types;
    std::shared_ptr<Bytecode> bytecode;

    std::string to_string();    
    std::string functions_as_strings();
    std::string exports_as_strings();
    std::string types_as_strings();
};


#endif //WASM_RUNTIME_MODULE_H
