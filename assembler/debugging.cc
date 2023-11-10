#include "parser.hh"
#include <iostream>

std::ostream& operator<<(std::ostream &os, const std::shared_ptr<GlobalVariable> &target) {
    os << "GlobalVariable [name: '" << target->name
        << "', type: '" << target->type
        << "', value: '" << target->value << "']";
    return os;
}

std::ostream& operator<<(std::ostream &os, const std::shared_ptr<Parameter> &target) {
    os << "Function [name: " << target->name << "]";
    return os;
}

std::ostream& operator<<(std::ostream &os, const std::shared_ptr<Function> &target) {
    os << "Function [name: " << target->name;
    if (!target->returns.empty())
    {
        os << ", returns: (";
        for (auto &type : target->returns)
            os << type << ',';
        os << ")";
    }
    os << "]";
    if (!target->params.empty())
        os << '\n';
    for (auto &param : target->params)
        os << param << '\n';
    return os;
}
