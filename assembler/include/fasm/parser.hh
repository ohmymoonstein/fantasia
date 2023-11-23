#pragma once

#include "lexer.hh"
#include <memory>
#include <list>
#include <fasm/ast.hh>

class Parser {
    public:
        Parser(Tokenizer &);
        ~Parser() = default;
        std::shared_ptr<Program> parse();

    protected:
        Tokenizer &tokenizer_;

        std::shared_ptr<Variable> parse_variable();
        std::shared_ptr<Function> parse_function();
        void parse_parameter_list(std::list<std::shared_ptr<Parameter>> &);
        void parse_type_list(std::list<TypeInfo> &);
        std::shared_ptr<Parameter> parse_parameter();
        void parse_function_body( std::list<std::shared_ptr<Instruction>> &result );
};