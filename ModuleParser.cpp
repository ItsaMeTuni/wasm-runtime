//
// Created by lucas on 3/23/22.
//

#include "ModuleParser.h"
#include <algorithm>

Module ModuleParser::parse() {
    Module module = Module();
    module.bytecode = bytecode;

    std::vector<Section> sections = read_sections();

    for(int i = 0; i < sections.size(); i++) {
        Section& section = sections.at(i);

        printf("reading section %d of id %d\n", i, section.id);

        if(section.id == SECTION_ID_EXPORTS) {
            module.exports = read_exports(section);
        } else if(section.id == SECTION_ID_FUNCTIONS) {
            module.functions = read_functions(sections);
        } else if(section.id == SECTION_ID_TYPES) {
            module.types = read_types(section);
        }
    }

    return module;
}

std::vector<Section> ModuleParser::read_sections() {
    // start at ninth byte in order to skip magic number and version
    // (first 8 bytes)
    u32 offset = 8;

    std::vector<Section> sections;

    while(offset < bytecode->size()) {
        char section_id = bytecode[offset];
        offset++;

        u32 section_size = bytecode->read_u32(offset);

        Section section = { .id = section_id, .offset = offset, .length = section_size };
        sections.push_back(section);

        offset += section_size;
    }

    return sections;
}

std::vector<Export> ModuleParser::read_exports(Section& section) {
    u32 offset = section.offset;

    u32 exports_count = bytecode->read_u32(offset);

    printf("%d exports found\n", exports_count);

    std::vector<Export> exports;

    for(u32 i = 0; i < exports_count; i++) {
        std::string name = bytecode->read_string(offset);

        char type = bytecode->at(offset);
        offset++;

        u32 exportee_idx = bytecode->read_u32(offset);

        Export export_ = { .name = name, .type = type, .exportee_idx = exportee_idx };

        printf("read export: name '%s', type 0x%02x, exportee index %d\n", name.c_str(), type, exportee_idx);
    }

    return exports;
}


u32 ModuleParser::find_function_body_offset(Section& code_section, u32 function_idx) {
    u32 offset = code_section.offset;
    u32 function_count = bytecode->read_u32(offset);

    for(u32 i = 0; i < function_count; i++) {
        printf("finding body size, offset is 0x%2x\n", offset);
        u32 body_size = bytecode->read_u32(offset);

        if (i == function_idx) {
            return offset;
        }

        offset += body_size;
    }

    // TODO: throw error

    return 0;
}

std::vector<Type> ModuleParser::read_types(Section& section) {
    printf("reading types\n");

    u32 offset = section.offset;

    u32 types_len = bytecode->read_u32(offset);

    std::vector<Type> types;

    for(u32 type_idx = 0; type_idx < types_len; type_idx++) {
        char type_code = bytecode->at(offset);
        if(type_code != TYPE_FUNCTION) {
            continue;
        }

        std::vector<char> params = bytecode->read_vector(&Bytecode::read_char, offset);
        std::vector<char> results = bytecode->read_vector(&Bytecode::read_char, offset);

        Type type = { .params = params, .results = results };
        types.push_back(type);
    }

    return types;
}

std::vector<Function> ModuleParser::read_functions(std::vector<Section>& sections) {
    printf("reading functions\n");

    Section* functions_section = find_section_by_id(SECTION_ID_FUNCTIONS);
    Section* code_section = find_section_by_id(SECTION_ID_CODE);

    u32 offset = functions_section->offset;

    u32 functions_count = bytecode->read_u32(offset);
    printf("%d functions found\n", functions_count);

    std::vector<Function> functions;

    for(u32 function_idx = 0; function_idx < functions_count; function_idx++) {
        u32 type_idx = bytecode->read_u32(offset);
        u32 function_body_offset = find_function_body_offset(*code_section, function_idx);

        Function function = {.offset = function_body_offset, .type_idx = type_idx};

        printf("read function: type idx %d, offset 0x%2x\n", type_idx, function_body_offset);
    }

    return functions;
}

Section *ModuleParser::find_section_by_id(std::vector<Section>& sections, char id) {
    auto it = find_if(
        sections.begin(),
        sections.end(),
        [](const Section& section) { return section.id == SECTION_ID_FUNCTIONS; }
    );

    if(it == sections.end()) {
        // TODO: throw error instead
        return nullptr;
    } else {
        return it.base();
    }
}