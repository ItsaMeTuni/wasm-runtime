#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "bytecode.h"
#include "runtime.h"

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
    Store store = make_store(module);

    invoke(&store, "main");
    step(&store);
    step(&store);
    step(&store);
    print_stack(&store);

    //printf("module:\n");

    //printf("sections:\n");
    //for(int i = 0; i < section_count; i++) {
    //    printf("id: %x, offset: %x, length: %x\n", sections[i].id, sections[i].offset, sections[i].length);
    //}

    //printf("\n");

    return 0;
}
