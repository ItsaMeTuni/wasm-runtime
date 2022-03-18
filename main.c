#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0

typedef char bool;

typedef struct {
    char id;
    unsigned int offset;
    unsigned int length;
} Section;

typedef struct {
    unsigned int idx;
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
    Function* functions;
    unsigned int functions_len;

    Export* exports;
    unsigned int exports_len;
} Module;

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
    
    *out = &bytecode[offset + bytes_read];

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

Export* read_exports(Section *section, char *bytecode) {
    printf("reading exports\n");
    unsigned int offset = section->offset;

    unsigned int exports_count = 0;
    offset += read_u32(bytecode, offset, &exports_count);

    printf("%d exports found\n", exports_count);
   
    Export *exports = (Export*)malloc(sizeof(Export) * exports_count);

    for(int i = 0; i < exports_count; i++) {
        char *name;
        offset += read_string(bytecode, offset, &name);

        char type = bytecode[offset];
        offset++;
        
        unsigned int exportee_idx;
        offset += read_u32(bytecode, offset, &exportee_idx);

        exports[i] = (Export){ .name = name, .type = type, .exportee_idx = exportee_idx };

        printf("read export: name '%s', type 0x%02x, exportee index %d\n", name, type, exportee_idx);
    } 

    return exports;
}

Module* read_module(char *bytecode, unsigned int bytecode_len) {
    Module* module = (Module*)malloc(sizeof(Module));

    Section* sections;
    unsigned short sections_len = read_sections(bytecode, bytecode_len, &sections);


    for(int i = 0; i < sections_len; i++) {
        Section section = sections[i];
        
        printf("reading section %d of id %d\n", i, section.id);

        if(section.id == 7) {
            module->exports = read_exports(&section, bytecode);
        }
    }

    free(sections);
}

int main() {
   
    int fd = open("code.wasm", O_RDONLY);
    int len = lseek(fd, 0, SEEK_END);
    char *data = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
    
    printf("successfully read file\n");

    //Section* sections;
    //unsigned short section_count = read_sections(data, len, &sections);
   
    printf("dump:\n");
    for(int i = 0; i < len; i++) {
        printf("%02x ", data[i]);
    }
    printf("\n");

    Module* module = read_module(data, len);
    //printf("module:\n");


    //printf("sections:\n");
    //for(int i = 0; i < section_count; i++) {
    //    printf("id: %x, offset: %x, length: %x\n", sections[i].id, sections[i].offset, sections[i].length);
    //}

    //printf("\n");

    return 0;
}
