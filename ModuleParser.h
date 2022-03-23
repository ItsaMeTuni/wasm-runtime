#ifndef WASM_RUNTIME_MODULEPARSER_H
#define WASM_RUNTIME_MODULEPARSER_H


#include <utility>

#include "bytecode.h"
#include "Module.h"


class ModuleParser {
    Bytecode bytecode;

    ModuleParser(Bytecode bytecode) : bytecode(std::move(bytecode)) { }

    Module parse();
};


#endif //WASM_RUNTIME_MODULEPARSER_H
