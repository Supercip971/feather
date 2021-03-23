#ifndef A24A3278_6B0E_4271_9600_1A363FB01DC1
#define A24A3278_6B0E_4271_9600_1A363FB01DC1
#include "arch/compiler_asm_generator.h"
#include <string>

enum class token_type : int
{
    NULL_TOKEN,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    INT_VALUE,
    END_OF_STREAM,
};

class token
{

    token_type _value = token_type::NULL_TOKEN;
    int _raw_value;

public:
    token(token_type value) : _value(value){};
    token(token_type value, int raw_val) : _value(value), _raw_value(raw_val){};
    std::string to_str();
    token_type &value() { return _value; };
    int &raw() { return _raw_value; };
};

enum class ast_type : int
{
    ADD,
    SUBSTRACT,
    DIVIDE,
    MULTIPLY,
    INT_VALUE,
    NULL_TYPE
};

std::string ast_to_str(ast_type type);

ast_type ast_from_token(token_type type);

class ast_node
{
public:
    ast_type type = ast_type::NULL_TYPE;
    int value = 0;
    ast_node *left = nullptr;
    ast_node *right = nullptr;
    void print();
    void destroy();

    int interpret();
    int generate(asm_generator *generator);
    static ast_node *create_node(ast_type type, ast_node *left,
                                 ast_node *right, int intvalue)
    {
        ast_node *v = new ast_node();

        v->left = left;
        v->right = right;
        v->value = intvalue;
        v->type = type;
        return v;
    }
    static ast_node *create_last(ast_type type, int intvalue)
    {
        return ast_node::create_node(type, nullptr, nullptr, intvalue);
    }

    static ast_node *create_single(ast_type type, ast_node *single, int intvalue)
    {
        return ast_node::create_node(type, single, nullptr, intvalue);
    }
};
// check for 0 precedence
int precedence(ast_type type);
// don't check for 0 precedence
int fprecedence(ast_type type);
#endif /* A24A3278_6B0E_4271_9600_1A363FB01DC1 */
