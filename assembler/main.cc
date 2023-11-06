#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <exception>

enum Tokens {
    TOK_EOF,
    TOK_DOT,
    TOK_FUNC,
    TOK_GLOBAL,
    TOK_IDENTIFIER,
    TOK_LOCAL,
    TOK_OPCODE,  // instructions
    TOK_NAME, // $name
    TOK_LBRACKET, // TODO: replace brackets by 'end' token
    TOK_RBRACKET,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_COMMA,
    TOK_INTEGER,
    TOK_COMMENT,
};

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
    TKLIT(TOK_LBRACKET),
    TKLIT(TOK_RBRACKET),
    TKLIT(TOK_LPAREN),
    TKLIT(TOK_RPAREN),
    TKLIT(TOK_COMMA),
    TKLIT(TOK_INTEGER),
    TKLIT(TOK_COMMENT),
};

class Scanner {
    public:
        Scanner(const std::string_view &code) : content_(code) {
            cursor_ = content_.begin();
        }
        ~Scanner() = default;

        char get() {
            if (cursor_ == content_.end())
                return 0;
            return *cursor_++;
        }

        char peek() {
            if (cursor_ == content_.end())
                return 0;
            return *cursor_;
        }

        void skip_spaces() {
            // TODO: return line breaks for non-empty lines
            while (isspace(*cursor_)) cursor_++;
        }

    private:
        std::string::iterator cursor_;
        std::string content_;
};

struct Token {
    int type; // TOK_*
    std::string literal;

    Token() : type(TOK_EOF) {}
    Token(int type) : type(type) {}
    Token(int type, const std::string_view &literal) : type(type), literal(literal) {}
};

class Tokenizer {
    public:
        Tokenizer( Scanner &scanner ) : scanner_(scanner) {}
        Tokenizer() = default;

        Token get() {
            scanner_.skip_spaces();
            auto c = scanner_.get();
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                return capture_identifier(c);
            if ((c >= '0' && c <= '9'))
                return capture_integer(c);
            // TODO: translate line breaks as 'end' token
            switch (c) {
                case 0:
                    return Token(TOK_EOF);
                case '{':
                    return Token(TOK_LBRACKET, "{");
                case '}':
                    return Token(TOK_RBRACKET, "}");
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
                    throw std::runtime_error("Token not recognized");
            }
        }

        Token capture_name() {
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

        Token capture_identifier( char c ) {
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

        Token capture_integer( char c ) {
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

    private:
        Scanner &scanner_;
};

int main(int argc, char **argv) {
    std::ifstream input(argv[1]);
    if (input.good()) {
        std::string content;

        input.seekg(0, std::ios_base::end);
        auto size = input.tellg();
        input.seekg(0, std::ios_base::beg);

        content.resize(size);
        input.read(content.data(), size);

        Scanner scanner(content);
        Tokenizer tokenizer(scanner);

        Token token;
        while ((token = tokenizer.get()).type != TOK_EOF)
        {
            std::cout << TOKENS[token.type];
            if (token.type == TOK_NAME || token.type == TOK_IDENTIFIER)
            std::cout << " " << token.literal;
            std::cout << '\n';
        }
    }
    return 0;
}