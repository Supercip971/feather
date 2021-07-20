#ifndef FLANG_AST_H
#define FLANG_AST_H
#include "cstar_token.h"
#include <vector>
enum ast_types
{
    AST_NULL = 0,
    AST_ADD = 1,
    AST_SUBSTRACT = 2,
    AST_MULTIPLY = 3,
    AST_DIVIDE = 4,
    AST_EQUAL = 5,
    AST_PARENTHESIS_START = 6,
    AST_PARENTHESIS_END = 7,
    AST_END_OF_LINE = 8,
    AST_INT_VALUE = 9,
    AST_KEYWORD = 10,
    AST_VARIABLE_DEF = 11,
    AST_FUNCTION_DEF = 12,
    AST_FUNCTION_TYPE = 13, // in function def, for exemple void(int, int)
    AST_FUNCTION_ARGUMENT = 14,
    AST_VAR_INSTANCE = 15,
    AST_CURLY_START = 16, // {
    AST_CURLY_END = 17,   // }
    AST_CODE = 18,
    AST_FUNC_CALL = 19,
    AST_COMMA = 20, // ,
    AST_IF = 21,
    AST_ELSEIF = 22,
    AST_ELSE = 23,
    AST_WHILE = 24,
    AST_FOR = 25,
    AST_RETURN = 26,
    AST_BREAK = 27,
    AST_CONTINUE = 28,
    AST_COND_EQUAL = 29,
    AST_COND_NEQUAL = 30,
    AST_COND_SUPERIOR = 31,
    AST_COND_INFERIOR = 32,
    AST_COND_SUPERIOR_EQUAL = 33,
    AST_COND_INFERIOR_EQUAL = 34,
    

};

int precedence_from_ast_type(ast_types type);

class ast_node
{
    std::vector<ast_node> _sub_nodes;
    ast_types _type;
    token _token;

public:
    ast_node(){};

    ast_node(token token, ast_types type)
        : _type(type), _token(token){};

    ast_node(ast_types type)
        : _type(type), _token(){};

    void add_node(ast_node node)
    {
        _sub_nodes.push_back(node);
    }

    std::vector<ast_node> &get_subnodes() { return _sub_nodes; };

    bool is_leaf() const { return _sub_nodes.empty(); };
    ast_types type() const { return _type; };
    void type(ast_types t) { _type = t; };
    token token() const { return _token; };

    static ast_node create_from_token(class token tok);

    void debug_out_recursive(int depth);

    size_t interpret();
};

#endif //FLANG_AST_H
