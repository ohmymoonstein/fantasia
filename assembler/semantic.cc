#include "semantic.hh"
#include "util.hh"
#include <unordered_map>
#include <set>

struct TypeInfo {
    ValueTypes code;
    int64_t min_value;
    int64_t max_value;
};

static const std::unordered_map<std::string_view, TypeInfo> TYPES = {
    { "i32", {VT_I32, INT32_MIN, INT32_MAX} },
    { "u32", {VT_U32, 0, UINT32_MAX} },
};

static ValueTypes validate_type( const std::string &type ) {
    // validate and translate variable type
    auto it = TYPES.find(type);
    if (it == TYPES.end())
        throw std::runtime_error(util_format("Invalid storage type '%s'", type.c_str()));
    return it->second.code;
}

Semantic::Semantic(std::shared_ptr<Program> program) : program_(program) {

}

void Semantic::validate() {
    for (auto &var : program_->variables)
        validate_variable(var);
    for (auto &func : program_->functions)
        validate_function(func);
}

void Semantic::validate_variable(std::shared_ptr<Variable> &target) {
    // validate and translate variable type
    auto it = TYPES.find(target->type);
    if (it == TYPES.end())
        throw std::runtime_error(util_format("Invalid storage type '%s'", target->type.c_str()));
    target->stype = it->second.code;
    // validate value range
    int64_t value = strtol(target->value.c_str(), nullptr, 10);
    if (value < it->second.min_value || value > it->second.max_value)
        throw std::runtime_error(util_format("Value '%s' out of range for '%s'",
            target->value.c_str(), target->type.c_str()));
}

void Semantic::validate_function(std::shared_ptr<Function> &target) {
    std::set<std::string_view> names;

    // parameters
    for (auto &param : target->params) {
        param->stype = validate_type(param->type);
        if (names.find(param->name) != names.end())
            throw std::runtime_error(util_format("Name '%s' already defined",
                param->name.c_str()));
        names.insert(param->name);
    }
    // local variable
    for (auto &var : target->variables) {
        validate_variable(var);
        if (names.find(var->name) != names.end())
            throw std::runtime_error(util_format("Name '%s' already defined",
                var->name.c_str()));
        names.insert(var->name);
    }
    // local constants
    for (auto &var : target->constants) {
        validate_variable(var);
        if (names.find(var->name) != names.end())
            throw std::runtime_error(util_format("Name '%s' already defined",
                var->name.c_str()));
        names.insert(var->name);
    }
    // function body
    for (auto &instr : target->body) {
        validate_instruction(instr);
    }
}

void Semantic::validate_instruction(std::shared_ptr<Instruction> &target) {
    if (!target->immediate.empty()) {
        int64_t value = strtol(target->immediate.c_str(), nullptr, 10);
        if (value < 0 || value > 1023) // 10-bits
            throw std::runtime_error(util_format("Immediate value out of range '%s'",
                target->immediate.c_str()));
    }
}