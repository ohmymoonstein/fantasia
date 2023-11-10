#include "lexer.hh"
#include <exception>
#include <stdexcept>
#include <array>
#include <iostream>
#include <cstdarg>

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
};

struct Keyword {
    const char *literal;
    TokenType type;
};

const std::array<Keyword, 4> KEYWORDS = {{
    {"global", TOK_GLOBAL},
    {"local", TOK_LOCAL},
    {"end", TOK_END},
    {"func", TOK_FUNC},
}};

static std::string format(const char *format, ...) {
    char buffer[512];
    va_list args;
    va_start(args, format);
	vsnprintf(buffer, sizeof(buffer) - 1, format, args);
	va_end(args);
    buffer[sizeof(buffer) -1] = 0;
    return buffer;
}

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

Token Tokenizer::get() {
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

Token Tokenizer::next() {
    scanner_.skip_spaces();
    auto c = scanner_.get();
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        auto token = capture_identifier(c);
        for (const Keyword &entry : KEYWORDS) {
            if (entry.literal == token.literal) {
                token.type = entry.type;
            }
        }
        return token;
    }
    if ((c >= '0' && c <= '9'))
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
        default:
            throw std::runtime_error(format("Symbol not recognized: %d", (int)c));
    }
}

Token Tokenizer::capture_name() {
    Token token(TOK_NAME);

    char c;
    do {
        c = scanner_.peek();
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            token.literal += c;
        else
            return token;
        scanner_.get();
    } while (true);
}

Token Tokenizer::capture_identifier( char c ) {
    Token token(TOK_IDENTIFIER);
    token.literal = c;

    // TODO: capture reserved words (e.g. global, local, func)

    do {
        c = scanner_.peek();
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
            token.literal += c;
        else
            return token;
        scanner_.get();
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
    if (type < TOK_EOF || type > TOK_LBREAK)
        return nullptr;
    return TOKENS[type];
}