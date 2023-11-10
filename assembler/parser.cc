#include "parser.hh"
#include "util.hh"
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
    auto token = tokenizer_.expected(TOK_FUNC);

    // variable name
    token = tokenizer_.advance();
    if (token.type != TOK_NAME)
        throw std::runtime_error("Unexpected token");
    result->name = token.literal;
    // left parentesis
    tokenizer_.expected(TOK_LPAREN);
    // parse parameters (handle the right parentesis too)
    parse_parameter_list(result->params);
    // left parentesis
    tokenizer_.expected(TOK_LPAREN);
    // parse types (handle the right parentesis too)
    parse_type_list(result->returns);
    // line break
    tokenizer_.expected(TOK_LBREAK);

    std::cout << result << '\n';

    return result;
}

void Parser::parse_parameter_list(std::list<std::shared_ptr<Parameter>> &result) {
    // left parentesis
    //tokenizer_.expected(TOK_LPAREN);

    Token token;
    while ((token = tokenizer_.peek()).type == TOK_NAME) {
        // parse parameter
        auto param = std::make_shared<Parameter>();
        param->name = token.literal;
        tokenizer_.advance();
        token = tokenizer_.expected(TOK_IDENTIFIER);
        param->type = token.literal;
        result.push_back( param );

        token = tokenizer_.peek();
        if (token.type == TOK_RPAREN) break;
        tokenizer_.expected(TOK_COMMA);
    }

    tokenizer_.advance();
}

void Parser::parse_type_list(std::list<std::string> &result) {
    Token token;
    while ((token = tokenizer_.advance()).type == TOK_IDENTIFIER) {
        result.push_back(token.literal);
        token = tokenizer_.peek(); // ) or ,
        if (token.type == TOK_RPAREN) break;
        tokenizer_.expected(TOK_COMMA);
    }

    tokenizer_.advance();
}

std::shared_ptr<GlobalVariable> Parser::parse_global_variable() {
    auto result = std::make_shared<GlobalVariable>();

    // global keyword
    auto token = tokenizer_.advance();
    if (token.type != TOK_GLOBAL)
        throw std::runtime_error("Unexpected token");

    // variable name
    token = tokenizer_.expected(TOK_NAME);
    result->name = token.literal;
    // variable type
    token = tokenizer_.expected(TOK_IDENTIFIER);
    result->type = token.literal;
    // initialization value
    token = tokenizer_.expected(TOK_INTEGER);
    result->value = token.literal;
    // line break
    tokenizer_.expected(TOK_LBREAK);

    std::cout << result << '\n';

    return result;
}