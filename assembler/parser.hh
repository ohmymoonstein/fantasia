#include "lexer.hh"
#include <memory>
#include <list>

struct Variable {
    std::string name;
    std::string type;
    std::string value;
    bool is_local = false;
};

struct Parameter {
    std::string name;
    std::string type;
};

struct Instruction {
    std::string opcode;
    std::string immediate;
};

struct Function {
    std::string name;
    std::list<std::shared_ptr<Parameter>> params;
    std::list<std::string> returns;
    std::list<std::shared_ptr<Variable>> variables;
    std::list<std::shared_ptr<Instruction>> body;
};


// Root node for our AST
struct Program {
    std::list<std::shared_ptr<Variable>> variables;
    std::list<std::shared_ptr<Function>> functions;
};

std::ostream& operator<<(std::ostream &, const std::shared_ptr<Program> &);
std::ostream& operator<<(std::ostream &, const std::shared_ptr<Variable> &);
std::ostream& operator<<(std::ostream &, const std::shared_ptr<Parameter> &);
std::ostream& operator<<(std::ostream &, const std::shared_ptr<Function> &);

class Parser {
    public:
        Parser(Tokenizer &);
        ~Parser() = default;
        std::shared_ptr<Program> parse();

    protected:
        Tokenizer &tokenizer_;

        std::shared_ptr<Variable> parse_variable();
        std::shared_ptr<Function> parse_function();
        void parse_parameter_list(std::list<std::shared_ptr<Parameter>> &);
        void parse_type_list(std::list<std::string> &);
        std::shared_ptr<Parameter> parse_parameter();
        void parse_function_body( std::list<std::shared_ptr<Instruction>> &result );
};