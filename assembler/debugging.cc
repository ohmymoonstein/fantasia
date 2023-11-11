#include "parser.hh"
#include <iostream>

std::ostream& operator<<(std::ostream &os, const std::shared_ptr<Program> &target) {
    for (auto &var : target->variables)
        os << var << '\n';
    for (auto &func : target->functions)
        os << func << '\n';
    return os;
}

std::ostream& operator<<(std::ostream &os, const std::shared_ptr<Variable> &target) {
    if (target->is_local)
        os << "    ";
    os << "Variable [name: '" << target->name
        << "', type: '" << target->type
        << "', value: '" << target->value << "']";
    return os;
}

std::ostream& operator<<(std::ostream &os, const std::shared_ptr<Parameter> &target) {
    os << "    Parameter [name: " << target->name << ", type: " << target->type << "]";
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
    if (!target->params.empty() || !target->variables.empty()) {
        os << '\n';
        for (auto &param : target->params)
            os << param << '\n';
        for (auto &var : target->variables)
            os << var << '\n';
    }
    return os;
}
