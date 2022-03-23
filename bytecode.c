#include "bytecode.h"

#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include "common.h"

#define SECTION_ID_EXPORTS 7
#define SECTION_ID_FUNCTIONS 3
#define SECTION_ID_CODE 10
#define SECTION_ID_TYPES 1

// Read a u32 (in LEB128 format).
// out: the output value
// returns: amount of bytes that were read
unsigned int read_u32(char *bytecode, unsigned int offset, unsigned int* out) {
    *out = 0;
    short bytes_processed = 0;
    bool has_next_byte = TRUE;

    while (has_next_byte) {
        char byte = bytecode[offset + bytes_processed];

        char zeroed_msb = (byte & 0b01111111);
        unsigned int padded = zeroed_msb << (bytes_processed * 7);
        *out |= padded;

        has_next_byte = (byte & 0b10000000) != 0; 
        bytes_processed++;
    } 

    return bytes_processed;
}

unsigned int read_string(char *bytecode, unsigned int offset, char** out) {
    unsigned int bytes_read = 0;

    unsigned int len = 0;
    bytes_read += read_u32(bytecode, offset, &len);
    
    *out = malloc(sizeof(char) * (len + 1));
    memcpy(*out, &bytecode[offset + bytes_read], sizeof(char) * len);
    (*out)[len] = '\0';

    return bytes_read + len;
}

unsigned short read_sections(char *bytecode, unsigned int bytecode_len, Section* sections[]) {
    // start at ninth byte in order to skip magic number and version
    // (first 8 bytes)
    unsigned int offset = 8;

    *sections = malloc(sizeof(Section) * 10);
    unsigned char section_index = 0;

    while(offset < bytecode_len) {
        char section_id = bytecode[offset];
        offset++;

        unsigned int section_size = 0;
        offset += read_u32(bytecode, offset, &section_size);
        //printf("section size %d\n", section_size);

        (*sections)[section_index] = (Section){ .id = section_id, .offset = offset, .length = section_size };
        offset += section_size;
        
        section_index++;
        //printf("create section %d %d %d\n", section_id, offset, section_size);
    }

    // return length of sections array
    return section_index;
}

unsigned int read_exports(Section *section, char *bytecode, Export **exports) {
    printf("reading exports\n");
    unsigned int offset = section->offset;

    unsigned int exports_count = 0;
    offset += read_u32(bytecode, offset, &exports_count);

    printf("%d exports found\n", exports_count);
   
    *exports = (Export*)malloc(sizeof(Export) * exports_count);

    for(unsigned int i = 0; i < exports_count; i++) {
        char *name;
        offset += read_string(bytecode, offset, &name);

        char type = bytecode[offset];
        offset++;
        
        unsigned int exportee_idx;
        offset += read_u32(bytecode, offset, &exportee_idx);

        (*exports)[i] = (Export){ .name = name, .type = type, .exportee_idx = exportee_idx };

        printf("read export: name '%s', type 0x%02x, exportee index %d\n", name, type, exportee_idx);
    } 

    return exports_count;
}

unsigned int find_function_body(Section *code_section, char *bytecode, unsigned int function_idx) {
    unsigned int offset = code_section->offset;

    unsigned int function_count = 0;
    offset += read_u32(bytecode, offset, &function_count);

    for(int i = 0; i < function_count; i++) {
        printf("finding body size, offset is 0x%2x\n", offset);
        unsigned int body_size = 0;
        offset += read_u32(bytecode, offset, &body_size);

        if (i == function_idx) {
            return offset;
        }

        printf("read body size, offset is 0x%2x, body size is %d\n", offset, body_size);
        offset += body_size;
        printf("added body size to offset, offset is 0x%2x\n", offset);
    }

    // TODO: throw error

    return 0;
}

unsigned int read_types(Section *section, char *bytecode, Type **types) {
    printf("reading types\n");

    unsigned int offset = section->offset;

    unsigned int types_len = 0;
    offset += read_u32(bytecode, offset, &types_len);

    *types = malloc(sizeof(Type) * types_len);

    for(unsigned int type_idx = 0; type_idx < types_len; type_idx++) {
        char type_code = bytecode[offset];
        if(type_code != TYPE_FUNCTION) {
            continue;
        }

        unsigned int params_len = 0;
        offset += read_u32(bytecode, offset, &params_len);

        char *params = malloc(sizeof(char) * params_len);
        for(unsigned int param_idx = 0; param_idx < params_len; param_idx++) {
            params[param_idx] = bytecode[offset];
        }

        unsigned int results_len = 0;
        offset += read_u32(bytecode, offset, &results_len);

        char *results = malloc(sizeof(char) * results_len);
        for(unsigned int result_idx = 0; result_idx < results_len; result_idx++) {
            results[result_idx] = bytecode[offset];
        }

        Type type = (Type) { .params = params, .params_len = params_len, .results = results, .results_len = results_len };
        (*types)[type_idx] = type;
    }

    return types_len;
}

unsigned int read_functions(Section *sections, int sections_len, char *bytecode, Function **functions) {
    printf("reading functions\n");

    Section *functions_section = NULL;
    Section *code_section = NULL;

    for(int i = 0; i < sections_len; i++) {
        Section *section = &sections[i];

        if(section->id == SECTION_ID_FUNCTIONS) {
            functions_section = section;
        } else if (section->id == SECTION_ID_CODE) {
            code_section = section;
        }
    }

    if(functions_section == NULL || code_section == NULL) {
        // TODO: throw error
    }


    unsigned int offset = functions_section->offset;

    unsigned int functions_count = 0;
    offset += read_u32(bytecode, offset, &functions_count);

    printf("%d functions found\n", functions_count);
   
    *functions = (Function*)malloc(sizeof(Function) * functions_count);
    
    for(unsigned int function_idx = 0; function_idx < functions_count; function_idx++) {
        unsigned int type_idx;
        offset += read_u32(bytecode, offset, &type_idx);

        (*functions)[function_idx].type_idx = type_idx;
        (*functions)[function_idx].offset = find_function_body(code_section, bytecode, function_idx);

        printf("read function: type idx %d, offset 0x%2x\n", type_idx, (*functions)[function_idx].offset);
    }

    return functions_count;
}

Module* read_module(char *bytecode, unsigned int bytecode_len) {
    Module* module = (Module*)malloc(sizeof(Module));
    module->bytecode = bytecode;
    module->bytecode_len = bytecode_len;

    Section* sections;
    unsigned short sections_len = read_sections(bytecode, bytecode_len, &sections);


    for(int i = 0; i < sections_len; i++) {
        Section section = sections[i];
        
        printf("reading section %d of id %d\n", i, section.id);

        if(section.id == SECTION_ID_EXPORTS) {
            module->exports_len = read_exports(&section, bytecode, &module->exports);
        } else if(section.id == SECTION_ID_FUNCTIONS) {
            module->functions_len = read_functions(sections, sections_len, bytecode, &module->functions);
        } else if(section.id == SECTION_ID_TYPES) {
            module->types_len = read_types(&section, bytecode, &module->types);
        }
    }

    free(sections);

    return module;
}


