#pragma once

#include <string>

enum TokenType {
    TOK_EOF,
    TOK_DOT,
    TOK_FUNC,
    TOK_GLOBAL,
    TOK_IDENTIFIER,
    TOK_LOCAL,
    TOK_OPCODE,  // instructions
    TOK_NAME, // $name
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_COMMA,
    TOK_INTEGER,
    TOK_COMMENT,
    TOK_END,
    TOK_LBREAK,
    TOK_INVALID,
};

struct Token {
    TokenType type; // TOK_*
    std::string literal;

    Token() : type(TOK_EOF) {}
    Token(TokenType type) : type(type) {}
    Token(TokenType type, const std::string_view &literal) : type(type), literal(literal) {}
    static const char *name(TokenType type);
};

class Scanner {
    public:
        Scanner(const std::string_view &code);
        ~Scanner() = default;
        char get();
        char peek();
        void skip_spaces();

    private:
        std::string::iterator cursor_;
        std::string content_;
        bool has_content_ = false;
};

class Tokenizer {
    public:
        Tokenizer( Scanner &scanner );
        Tokenizer() = default;

        Token peek();
        Token advance();
        Token expected( TokenType type );
        Token capture_name();
        Token capture_identifier( char c );
        Token capture_integer( char c );

    private:
        Scanner &scanner_;
        TokenType last_type_;
        Token current_;

        Token next();
};