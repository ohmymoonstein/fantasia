#include "parser.hh"
#include "util.hh"
#include <stdexcept>
#include <iostream>

Parser::Parser(Tokenizer &tokenizer) : tokenizer_(tokenizer) {

}

std::shared_ptr<Program> Parser::parse() {
    auto result = std::make_shared<Program>();
    bool done = false;

    do {
        auto token = tokenizer_.advance();
        switch (token.type)
        {
            case TOK_GLOBAL:
                result->variables.push_back( parse_variable() );
                break;
            case TOK_FUNC:
                result->functions.push_back( parse_function() );
                break;
            case TOK_EOF:
                done = true;
                break;
            default:
                throw std::runtime_error(util_format("Unexpected token %s", Token::name(token.type)));
        }
    } while (!done);

    return result;
}

std::shared_ptr<Function> Parser::parse_function() {
    auto result = std::make_shared<Function>();

    // variable name
    auto token = tokenizer_.expected(TOK_NAME);
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

    // local variables
    while ((token = tokenizer_.peek()).type == TOK_LOCAL) {
        tokenizer_.advance();
        auto var = parse_variable();
        var->kind = VAR_LOCAL;
        result->variables.push_back(var);
    }
    // local constants
    while ((token = tokenizer_.peek()).type == TOK_CONST) {
        tokenizer_.advance();
        auto var = parse_variable();
        var->kind = VAR_CONST;
        result->constants.push_back(var);
    }

    parse_function_body(result->body);

    tokenizer_.expected(TOK_END);
    tokenizer_.expected(TOK_LBREAK);

    return result;
}

void Parser::parse_function_body( std::list<std::shared_ptr<Instruction>> &result ) {
    Token token;
    while ((token = tokenizer_.peek()).type == TOK_OPCODE) {
        // instruction (opcode)
        auto instr = std::make_shared<Instruction>();
        instr->opcode = token.literal;
        tokenizer_.advance();
        // check for immediate
        token = tokenizer_.peek();
        if (tokenizer_.peek().type == TOK_INTEGER) {
            instr->immediate = token.literal;
            tokenizer_.advance();
        }
        result.push_back(instr);
        tokenizer_.expected(TOK_LBREAK);
    }
}

void Parser::parse_parameter_list(std::list<std::shared_ptr<Parameter>> &result) {
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

std::shared_ptr<Variable> Parser::parse_variable() {
    auto result = std::make_shared<Variable>();

    // variable name
    auto token = tokenizer_.expected(TOK_NAME);
    result->name = token.literal;
    // variable type
    token = tokenizer_.expected(TOK_IDENTIFIER);
    result->type = token.literal;
    // initialization value
    token = tokenizer_.expected(TOK_INTEGER);
    result->value = token.literal;
    // line break
    tokenizer_.expected(TOK_LBREAK);

    return result;
}