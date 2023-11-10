#include "parser.hh"
#include <stdexcept>
#include <iostream>

Parser::Parser(Tokenizer &tokenizer) : tokenizer_(tokenizer) {

}

std::shared_ptr<Program> Parser::parse() {
    auto result = std::make_shared<Program>();

    do {
        auto token = tokenizer_.peek();
        switch (token.type)
        {
            case TOK_GLOBAL:
                result->variables.push_back( parse_global_variable() );
                break;
            case TOK_FUNC:
                result->functions.push_back( parse_function() );
                break;
            case TOK_EOF:
                break;
            default:
                throw std::runtime_error("Unexpected token");
        }
    } while (true);

    return result;
}

std::shared_ptr<Function> Parser::parse_function() {
    auto result = std::make_shared<Function>();

    // func keyword
    auto token = tokenizer_.get();
    if (token.type != TOK_FUNC)
        throw std::runtime_error("Unexpected token");

    // variable name
    token = tokenizer_.get();
    if (token.type != TOK_NAME)
        throw std::runtime_error("Unexpected token");
    result->name = token.literal;
    // left parentesis
    token = tokenizer_.peek();
    if (token.type != TOK_LPAREN)
        throw std::runtime_error("Unexpected token");
    // parse parameters (handle the right parentesis too)
    parse_parameter_list(result->params);
    // left parentesis
    token = tokenizer_.peek();
    if (token.type != TOK_LPAREN)
        throw std::runtime_error("Unexpected token");
    // parse types (handle the right parentesis too)
    parse_type_list(result->returns);
    // line break
    token = tokenizer_.get();
    if (token.type != TOK_LBREAK)
        throw std::runtime_error("Unexpected token");

    std::cout << result << '\n';

    return result;
}

void Parser::parse_parameter_list(std::list<std::shared_ptr<Parameter>> &result) {
    // left parentesis
    auto token = tokenizer_.get();
    if (token.type != TOK_LPAREN)
        throw std::runtime_error("Unexpected token");

    while ((token = tokenizer_.peek()).type == TOK_NAME) {
        result.push_back( parse_parameter() );
        token = tokenizer_.peek();
        if (token.type == TOK_RPAREN) break;
        token = tokenizer_.get();
        if (token.type != TOK_COMMA)
            throw std::runtime_error("Unexpected token");
    }

    token = tokenizer_.get();
    if (token.type != TOK_RPAREN)
        throw std::runtime_error("Unexpected token");
}

std::shared_ptr<Parameter> Parser::parse_parameter() {
    return nullptr;
}

void Parser::parse_type_list(std::list<std::string> &result) {
    // left parentesis
    auto token = tokenizer_.get();
    if (token.type != TOK_LPAREN)
        throw std::runtime_error("Unexpected token");

    while ((token = tokenizer_.get()).type == TOK_IDENTIFIER) {
        result.push_back(token.literal);
        token = tokenizer_.get(); // ( or ,
        if (token.type == TOK_RPAREN) break;
        if (token.type != TOK_COMMA)
            throw std::runtime_error("Unexpected token");
    }

    if (token.type != TOK_RPAREN)
        throw std::runtime_error("Unexpected token");
}

std::shared_ptr<GlobalVariable> Parser::parse_global_variable() {
    auto result = std::make_shared<GlobalVariable>();

    // global keyword
    auto token = tokenizer_.get();
    if (token.type != TOK_GLOBAL)
        throw std::runtime_error("Unexpected token");

    // variable name
    token = tokenizer_.get();
    if (token.type != TOK_NAME)
        throw std::runtime_error("Unexpected token");
    result->name = token.literal;
    // variable type
    token = tokenizer_.get();
    if (token.type != TOK_IDENTIFIER)
        throw std::runtime_error("Unexpected token");
    result->type = token.literal;
    // initialization value
    token = tokenizer_.get();
    if (token.type != TOK_INTEGER)
        throw std::runtime_error("Unexpected token");
    result->value = token.literal;
    // line break
    token = tokenizer_.get();
    if (token.type != TOK_LBREAK)
        throw std::runtime_error("Unexpected token");

    std::cout << result << '\n';

    return result;
}