#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <exception>
#include <fasm/lexer.hh>
#include <fasm/parser.hh>
#include <fasm/semantic.hh>
#include "debugging.hh"

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
#if 0
        char c;
        while ((c = scanner.get()) != 0)
        {
            std::cout << c;
        }
        return 0;
#endif
        Tokenizer tokenizer(scanner);
#if 0
        Token token;
        while ((token = tokenizer.advance()).type != TOK_EOF)
        {
            std::cout << Token::name(token.type);
            if (token.type == TOK_NAME || token.type == TOK_IDENTIFIER || token.type == TOK_INTEGER)
                std::cout << " " << token.literal;
            if (token.type == TOK_STRING)
                std::cout << " \"" << token.literal << "\"";
            std::cout << '\n';
        }
        std::cout << "Done\n";
#else
        Parser parser(tokenizer);
        auto program = parser.parse();
        std::cout << program << '\n';

        Semantic semantic(program);
        semantic.validate();
#endif
    }
    return 0;
}
