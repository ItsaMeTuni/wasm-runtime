#ifndef WASM_RUNTIME_MODULEPARSER_H
#define WASM_RUNTIME_MODULEPARSER_H


#include <utility>

#include "bytecode.h"
#include "Module.h"

#define SECTION_ID_EXPORTS 7
#define SECTION_ID_FUNCTIONS 3
#define SECTION_ID_CODE 10
#define SECTION_ID_TYPES 1

class ModuleParser {
    std::shared_ptr<Bytecode> bytecode;

public:
    explicit ModuleParser(std::shared_ptr<Bytecode> bytecode) : bytecode(bytecode) { }

    std::shared_ptr<Module> parse();
    std::vector<Section> read_sections();
    std::vector<Export> read_exports(Section& section);
    u32 find_function_body_offset(Section& code_section, unsigned int function_idx);
    std::vector<Type> read_types(Section& section);
    std::vector<Function> read_functions(std::vector<Section>& sections);
    static Section* find_section_by_id(std::vector<Section>& sections, char id);
    std::string to_string();
};


#endif //WASM_RUNTIME_MODULEPARSER_H
