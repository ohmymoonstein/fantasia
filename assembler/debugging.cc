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
    if (target->kind == VAR_LOCAL)
        os << "    Varibale";
    else
    if (target->kind == VAR_CONST)
        os << "    Constant";
    else
        os << "Variable";
    os << " [name: '" << target->name
        << "', type: '" << target->type.type
        << "', value: '" << target->value << "']";
    return os;
}

std::ostream& operator<<(std::ostream &os, const std::shared_ptr<Parameter> &target) {
    os << "    Parameter [name: " << target->name << ", type: " << target->type.type << "]";
    return os;
}

std::ostream& operator<<(std::ostream &os, const std::shared_ptr<Instruction> &target) {
    os << "    Instruction [opcode: " << target->opcode;
    if (!target->immediate.empty())
        os << ", immediate: " << target->immediate;
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream &os, const std::shared_ptr<Function> &target) {
    os << "Function [name: " << target->name;
    if (!target->returns.empty())
    {
        os << ", returns: (";
        for (auto &type : target->returns)
            os << type.type << ',';
        os << ")";
    }
    os << "]";
    if (!target->params.empty()) {
        os << '\n';
        for (auto &param : target->params)
            os << param << '\n';
    }
    if (!target->variables.empty()) {
        os << '\n';
        for (auto &var : target->variables)
            os << var << '\n';
    }
    if (!target->constants.empty()) {
        os << '\n';
        for (auto &var : target->constants)
            os << var << '\n';
    }
    if (!target->body.empty()) {
        os << '\n';
        for (auto &instr : target->body)
            os << instr << '\n';
    }
    return os;
}
