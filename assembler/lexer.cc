#include <fasm/lexer.hh>
#include "util.hh"
#include <exception>
#include <stdexcept>
#include <array>
#include <iostream>
#include <cstdarg>
#include <fasm/ast.hh>

#define TKLIT(x) #x

static const std::array TOKENS{
    TKLIT(TOK_EOF),
    TKLIT(TOK_DOT),
    TKLIT(TOK_FUNC),
    TKLIT(TOK_GLOBAL),
    TKLIT(TOK_IDENTIFIER),
    TKLIT(TOK_LOCAL),
    TKLIT(TOK_OPCODE),
    TKLIT(TOK_NAME),
    TKLIT(TOK_LPAREN),
    TKLIT(TOK_RPAREN),
    TKLIT(TOK_COMMA),
    TKLIT(TOK_INTEGER),
    TKLIT(TOK_COMMENT),
    TKLIT(TOK_END),
    TKLIT(TOK_LBREAK),
    TKLIT(TOK_CONST),
    TKLIT(TOK_STRING),
    TKLIT(TOK_INVALID),
};

struct Keyword {
    const char *literal;
    TokenType type;
};

const std::array<Keyword, 5> KEYWORDS = {{
    {"global", TOK_GLOBAL},
    {"local", TOK_LOCAL},
    {"const", TOK_CONST},
    {"end", TOK_END},
    {"func", TOK_FUNC},
}};

const std::unordered_map<std::string_view, OpcodeDetail> OPCODES = {
    { "add",    {OPC_ADD, } },
    { "sub",    {OPC_SUB, } },
    { "mul",    {OPC_MUL, } },
    { "sdiv",   {OPC_SDIV, } },
    { "udiv",   {OPC_UDIV, } },
    { "srem",   {OPC_SREM, } },
    { "urem",   {OPC_UREM, } },
    { "and",    {OPC_AND, } },
    { "or",     {OPC_OR, } },
    { "xor",    {OPC_XOR, } },
    { "shl",    {OPC_SHL, } },
    { "shr",    {OPC_SHR, } },
    { "rotl",   {OPC_ROTL, } },
    { "rotr",   {OPC_ROTR, } },
    { "eq",     {OPC_EQ, } },
    { "ne",     {OPC_NE, } },
    { "slt",    {OPC_SLT, } },
    { "sgt",    {OPC_SGT, } },
    { "sle",    {OPC_SLE, } },
    { "sge",    {OPC_SGE, } },
    { "ult",    {OPC_ULT, } },
    { "ugt",    {OPC_UGT, } },
    { "ule",    {OPC_ULE, } },
    { "uge",    {OPC_UGE, } },
    { "clz",    {OPC_CLZ, } },
    { "ctz",    {OPC_CTZ, } },
    { "cbit",   {OPC_CBIT, } },
    { "load",   {OPC_LOAD, } },
    { "store",  {OPC_STORE, } },
    { "ldc",    {OPC_LDC, IT_INT | IT_STR | IT_NAME } },
    { "ldv",    {OPC_LDV,  IT_INT } },
    { "ldo",    {OPC_LDO, } },
    { "ldz",    {OPC_LDZ, } },
    { "lget",   {OPC_LGET, IT_INT | IT_NAME } },
    { "lset",   {OPC_LSET, IT_INT | IT_NAME } },
    { "gget",   {OPC_GGET, IT_INT | IT_NAME } },
    { "gset",   {OPC_GSET, IT_INT | IT_NAME } },
    { "call",   {OPC_CALL, IT_INT | IT_NAME } },
    { "return", {OPC_RETURN, IT_INT } },
    { "jmp",    {OPC_JMP, IT_NAME } },
    { "jt",     {OPC_JT, IT_NAME } },
    { "jf",     {OPC_JF, IT_NAME } },
    { "drop",   {OPC_DROP, } },
    { "dup",    {OPC_DUP, } },
    { "nop",    {OPC_NOP, } },
    { "trap",   {OPC_TRAP, } },
    { "cat",    {OPC_CAT, } },
    { "fmt",    {OPC_FMT, } },
    { "len",    {OPC_LEN, } },
};

Scanner::Scanner(const std::string_view &code) : content_(code) {
    cursor_ = content_.begin();
}

char Scanner::get() {
    if (cursor_ == content_.end())
        return 0;
    if (*cursor_ == '\n')
    {
        while (isspace(*cursor_))
            cursor_++;
        return '\n';
    }
    return *cursor_++;
}

char Scanner::peek() {
    if (cursor_ == content_.end())
        return 0;
    return *cursor_;
}

void Scanner::skip_spaces() {
    if (cursor_ == content_.end())
        return;
    while (*cursor_ != '\n' && isspace(*cursor_))
        cursor_++;
}

Tokenizer::Tokenizer( Scanner &scanner ) : scanner_(scanner) {
    last_type_ = TOK_LBREAK;
    current_.type = TOK_INVALID;
}

Token Tokenizer::advance() {
    Token token;
    if (current_.type != TOK_INVALID) {
        token = current_;
        current_.type = TOK_INVALID;
        return token;
    }

    token = next();
    last_type_ = token.type;
    return token;
}

Token Tokenizer::expected( TokenType type ) {
    auto token = advance();
    if (token.type != type)
        throw std::runtime_error(
            util_format("Expected %s but found %s", Token::name(type), Token::name(token.type)));
    return token;
}

Token Tokenizer::expected( std::initializer_list<TokenType> types ) {
    auto token = advance();
    for (auto &item : types) {
        if (token.type == item) return token;
    }

    std::string tmp;
    for (auto &item : types) {
        tmp += Token::name(item);
        tmp += ' ';
    }

    throw std::runtime_error(
        util_format("Expected ( %s) but found %s", tmp.c_str(), Token::name(token.type)));
}

Token Tokenizer::next() {
    scanner_.skip_spaces();
    auto c = scanner_.get();
    // identifiers
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        auto token = capture_identifier(c);
        // translate keywords
        for (const Keyword &entry : KEYWORDS) {
            if (entry.literal == token.literal) {
                token.type = entry.type;
            }
        }
        // translate opcoes
        if (OPCODES.find(token.literal) != OPCODES.end()) {
            token.type = TOK_OPCODE;
        }
        return token;
    }
    if ((c >= '0' && c <= '9') || c == '+' || c == '-')
        return capture_integer(c);
    // TODO: translate line breaks as 'end' token
    switch (c) {
        case 0:
            return Token(TOK_EOF);
        case '\n':
            return Token(TOK_LBREAK, "\n");
        case '.':
            return Token(TOK_DOT, ".");
        case '(':
            return Token(TOK_LPAREN, ")");
        case ')':
            return Token(TOK_RPAREN, ")");
        case ',':
            return Token(TOK_COMMA, ",");
        case '$':
            return capture_name();
        case '"':
            return capture_string();
        default:
            throw std::runtime_error(util_format("Symbol not recognized: %d", (int)c));
    }
}

Token Tokenizer::capture_name() {
    Token token(TOK_NAME);

    // TODO: first char must be alphabetic

    char c;
    do {
        c = scanner_.peek();
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
            token.literal += c;
        else
            return token;
        scanner_.get();
    } while (true);
}

Token Tokenizer::capture_identifier( char c ) {
    Token token(TOK_IDENTIFIER);
    token.literal = c;

    do {
        c = scanner_.peek();
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
            token.literal += c;
        else
            return token;
        scanner_.get();
    } while (true);
}

Token Tokenizer::capture_string() {
    Token token(TOK_STRING);

    do {
        char c = scanner_.get();
        if (c == '\n' || c == 0)
            throw std::runtime_error(util_format("Unterminated string"));
        if (c != '"')
            token.literal += c;
        else
            return token;
    } while (true);
}

Token Tokenizer::capture_integer( char c ) {
    Token token(TOK_INTEGER);
    token.literal = c;

    do {
        c = scanner_.peek();
        if ((c >= '0' && c <= '9'))
            token.literal += c;
        else
            return token;
        scanner_.get();
    } while (true);
}

Token Tokenizer::peek() {
    if (current_.type == TOK_INVALID)
        current_ = next();
    return current_;
}

const char *Token::name(TokenType type) {
    if (type < TOK_EOF || type > TOK_INVALID)
        return nullptr;
    return TOKENS[type];
}