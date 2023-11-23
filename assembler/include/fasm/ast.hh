#pragma once

#include <string>
#include <list>
#include <memory>

enum VariableKinds {
    VAR_GLOBAL = 0,
    VAR_LOCAL = 1,
    VAR_CONST = 2,
};

enum ValueTypes {
    VT_INT = 0,
    VT_STR = 1,
};

struct TypeInfo {
    std::string type;
    ValueTypes stype = VT_INT;
};

struct Variable {
    std::string name;
    TypeInfo type;
    std::string value;
    ValueTypes value_type;
    VariableKinds kind = VAR_GLOBAL;
};

struct Parameter {
    std::string name;
    TypeInfo type;
};

enum ImmediateType {
    IT_INT = 1,
    IT_STR = 2,
    IT_NAME = 4,
};

struct ImmediateInfo {
    std::string literal;
    int value = -1;
    int type = IT_INT;
};

struct OpcodeInfo {
    std::string literal;
    int value = -1;
};

struct Instruction {
    OpcodeInfo opcode;
    ImmediateInfo immediate;
};

struct Function {
    std::string name;
    std::list<std::shared_ptr<Parameter>> params;
    std::list<TypeInfo> returns;
    std::list<std::shared_ptr<Variable>> variables;
    std::list<std::shared_ptr<Variable>> constants;
    std::list<std::shared_ptr<Instruction>> body;
};

// Root node for our AST
struct Program {
    std::list<std::shared_ptr<Variable>> variables;
    std::list<std::shared_ptr<Function>> functions;
};