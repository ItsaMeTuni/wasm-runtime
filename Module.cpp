#include "Module.h"

#include <string>
#include <sstream>

std::string Module::to_string() {
    std::stringstream ss;

    ss << "Module debug:\n"
        "\t Functions:\n"
        << functions_as_strings() <<
        "\t Exports:\n"
        << exports_as_strings() <<
        "\t Types:\n"
        << types_as_strings();

    return ss.str();
}

std::string Module::functions_as_strings() {
    std::stringstream ss;

    for(unsigned long i = 0; i < functions.size(); i++) {
        auto fn = functions[i];
        ss << "\t\tidx: " << i << ", offset: ";
        ss << "0x" << std::hex << (int)fn.offset;
        ss << ", type idx: " << fn.type_idx << "\n";
    }

    return ss.str();
}
std::string Module::exports_as_strings() {
    std::stringstream ss;
    printf("exports len is %ld\n", exports.size());
    for(unsigned long i = 0; i < exports.size(); i++) {
        auto _export = exports[i];
        ss << "\t\tname: " << _export.name << ", exportee idx: " << _export.exportee_idx << ", type" << _export.type << "\n";
    }

    return ss.str();

}
std::string Module::types_as_strings() {
    std::stringstream ss;

    for(unsigned long i = 0; i < types.size(); i++) {
        auto type = types[i];
        //std::string params(type.params.begin(), type.params.end());
        //std::string results(type.results.begin(), type.results.end());
        ss << "\t\tidx: " << i << ", params len: " << type.params.size() << ", results len: " << type.results.size() << "\n";//<< ", params: " << params << ", results: " << results;
    }

    return ss.str();

}

