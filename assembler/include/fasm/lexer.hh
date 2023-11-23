#pragma once

#include <string>
#include <unordered_map>

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
    TOK_CONST,
    TOK_STRING,
    TOK_INVALID,
};

enum Opcodes {
    OPC_ADD = 0,
    OPC_SUB,
    OPC_MUL,
    OPC_SDIV,
    OPC_UDIV,
    OPC_SREM,
    OPC_UREM,
    OPC_AND,
    OPC_OR,
    OPC_XOR,
    OPC_SHL,
    OPC_SHR,
    OPC_ROTL,
    OPC_ROTR,
    OPC_EQ,
    OPC_NE,
    OPC_SLT,
    OPC_SGT,
    OPC_SLE,
    OPC_SGE,
    OPC_ULT,
    OPC_UGT,
    OPC_ULE,
    OPC_UGE,
    OPC_CLZ,
    OPC_CTZ,
    OPC_CBIT,
    OPC_LOAD,
    OPC_STORE,
    OPC_LDC,
    OPC_LDV,
    OPC_LDO,
    OPC_LDZ,
    OPC_LGET,
    OPC_LSET,
    OPC_GGET,
    OPC_GSET,
    OPC_CALL,
    OPC_RETURN,
    OPC_JMP,
    OPC_JT,
    OPC_JF,
    OPC_DROP,
    OPC_DUP,
    OPC_NOP,
    OPC_TRAP,
    OPC_CAT,
    OPC_FMT,
    OPC_LEN,
};

struct OpcodeDetail {
    Opcodes opcode;
    int immediate_types = 0;
};

extern const std::unordered_map<std::string_view, OpcodeDetail> OPCODES;

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
        Token expected( std::initializer_list<TokenType> types );
        Token capture_name();
        Token capture_identifier( char c );
        Token capture_string();
        Token capture_integer( char c );

    private:
        Scanner &scanner_;
        TokenType last_type_;
        Token current_;

        Token next();
};