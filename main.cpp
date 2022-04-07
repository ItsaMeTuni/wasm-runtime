#include <fstream>
#include <iterator>
#include <filesystem>
#include <iostream>

#include "bytecode.h"
//#include "runtime.h"
#include "Module.h"
#include "ModuleParser.h"
#include "runtime.h"

std::vector<char> read_file(std::string path) {
    std::vector<char> bytes;

    std::ifstream file(path, std::ios::binary);

    file.seekg(0, std::ios::end);
    long len = file.tellg();
    bytes.resize(len);

    file.seekg(0, std::ios::beg);
    file.read(&bytes.front(), len);

    return bytes;
}

int main() {
    auto bytes = read_file(std::filesystem::current_path().append("code.wasm"));

    std::shared_ptr<Bytecode> bytecode = std::make_shared<Bytecode>(bytes);
    ModuleParser moduleParser(bytecode);
    std::shared_ptr<Module> module = moduleParser.parse();
    std::cout << module->to_string() << std::endl;
    Store store = Store(module);
    store.invoke(0);
    store.step();
    store.step();
    store.step();
    store.step();
    store.step();
    store.step();
    store.step();
    store.step();

    store.print_stack();
    return 0;
}
