#include "lexer.hh"
#include <memory>
#include <list>

struct GlobalVariable {
    std::string name;
    std::string type;
    std::string value;
};

struct Parameter {
    std::string name;
    std::string type;
};

struct Function {
    std::string name;
    std::list<std::shared_ptr<Parameter>> params;
    std::list<std::string> returns;
};


// Root node for our AST
struct Program {
    std::list<std::shared_ptr<GlobalVariable>> variables;
    std::list<std::shared_ptr<Function>> functions;
};

class Parser {
    public:
        Parser(Tokenizer &);
        ~Parser() = default;

        std::shared_ptr<Program> parse();
        std::shared_ptr<GlobalVariable> parse_global_variable();
        std::shared_ptr<Function> parse_function();
        void parse_parameter_list(std::list<std::shared_ptr<Parameter>> &);
        void parse_type_list(std::list<std::string> &);
        std::shared_ptr<Parameter> parse_parameter();


    protected:
        Tokenizer &tokenizer_;
};