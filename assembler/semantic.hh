#pragma once


#include <iostream>
#include "parser.hh"

class Semantic {
    public:
        Semantic(std::shared_ptr<Program> program);
        ~Semantic() = default;

        void validate();
        void validate_variable(std::shared_ptr<Variable> &target);
        void validate_function(std::shared_ptr<Function> &target);
        void validate_parameter(std::shared_ptr<Parameter> &target);
        void validate_instruction(std::shared_ptr<Instruction> &target);

    protected:
        std::shared_ptr<Program> program_;
};
