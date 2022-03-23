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
    std::vector<Function> functions;
    std::vector<Export> exports;
    std::vector<Type> types;
    std::unique_ptr<Bytecode> bytecode;
} Module;


#endif //WASM_RUNTIME_MODULE_H
