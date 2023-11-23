#include <fasm/semantic.hh>
#include "util.hh"
#include <unordered_map>
#include <set>

struct TypeDetails {
    ValueTypes code;
    int64_t min_value;
    int64_t max_value;
};

static ValueTypes validate_type( const std::string &type ) {
    // validate and translate variable type
    if (type == "int")
        return VT_INT;
    if (type == "str")
        return VT_STR;
    else
        throw std::runtime_error(util_format("Invalid storage type '%s'", type.c_str()));
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
    const char *literal = target->value.c_str();
    // validate and translate variable type
    target->type.stype = validate_type(target->type.type);

    if (target->type.stype == VT_STR && target->value_type != VT_STR)
        throw std::runtime_error(util_format("Expected string literal"));
    else
    if (target->type.stype == VT_INT) {
        if (target->value_type != VT_INT)
            throw std::runtime_error(util_format("Expected integer literal"));

        // validate and translate variable type
        target->type.stype = validate_type(target->type.type);
        // validate value range
        char *ptr = nullptr;
        int64_t value = strtol(literal, &ptr, 10);
        if (literal == ptr)
            throw std::runtime_error(util_format("Value '%s' it not a integer", value));

        // TODO: validate errors for 'strtol'

        if (value < INT32_MIN || value > UINT32_MAX)
            throw std::runtime_error(util_format("Value '%s' out of range for '%s'",
                literal, target->type.type.c_str()));
    }
}

void Semantic::validate_function(std::shared_ptr<Function> &target) {
    std::set<std::string_view> names;

    // parameters
    for (auto &param : target->params) {
        param->type.stype = validate_type(param->type.type);
        if (names.find(param->name) != names.end())
            throw std::runtime_error(util_format("Name '%s' already defined",
                param->name.c_str()));
        names.insert(param->name);
    }
    for (auto &type : target->returns) {
        type.stype = validate_type(type.type);
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
        validate_instruction(target, instr);
    }
}

void Semantic::validate_instruction(const std::shared_ptr<Function> &parent, std::shared_ptr<Instruction> &target) {
    const char *opcode = target->opcode.literal.c_str();

    // get information about the current immediate
    auto it = OPCODES.find(target->opcode.literal);
    if (it == OPCODES.end())
        throw std::runtime_error(util_format("Unknown instruction '%s'", opcode));
    target->opcode.value = it->second.opcode;
    // validate the immediate presence
    if (it->second.immediate_types != 0) {
        if (target->immediate.literal.empty())
            throw std::runtime_error(util_format("Expected immediate for '%s'", opcode));
        // validate the immediate type against the valid ones
        if ((it->second.immediate_types & target->immediate.type) == 0)
            throw std::runtime_error(util_format("Unsupported immediate type for '%s'", opcode));
        if (target->immediate.type == IT_INT) {
            // validate immediate value
            // TODO: validate errors for 'strtol'
            int64_t value = strtol(target->immediate.literal.c_str(), nullptr, 10);
            if (value < 0 || value > 1023) // 10-bits
                throw std::runtime_error(util_format("Immediate value out of range '%s'",
                    target->immediate.literal.c_str()));
            target->immediate.value = (int) value;
            // check for number of return values
            if (target->opcode.value == OPC_RETURN) {
                if (value != (int64_t) parent->returns.size())
                    throw std::runtime_error(util_format("Expected %d return values instead of %d",
                        (int) parent->returns.size(), (int) value));
            }
        }
    }
    if (it->second.immediate_types == 0 && !target->immediate.literal.empty())
        throw std::runtime_error(util_format("Unexpected immediate for '%s'", opcode));
}