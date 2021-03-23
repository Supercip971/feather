#include "parser.h"

ast_node *parser::expression(int lprecedence)
{
    ast_node *left, *right;
    ast_type type;
    left = start();

    if (current_token.value() == token_type::END_OF_STREAM)
    {
        return left;
    }

    type = ast_from_token(current_token.value());
    while (precedence(type) > lprecedence)
    {

        current_token = current_scanner.next_token();

        right = expression(fprecedence(type));

        left = ast_node::create_node(type, left, right, 0);

        type = ast_from_token(current_token.value());

        if (type == ast_type::NULL_TYPE)
        {
            return left;
        }
    }

    return left;
}
ast_node *parser::create_ast_node()
{

    current_token = current_scanner.next_token();
    return expression(0);
}
ast_node *parser::start()
{
    if (current_token.value() == token_type::INT_VALUE)
    {
        auto ret = ast_node::create_last(ast_type::INT_VALUE, current_token.raw());
        current_token = current_scanner.next_token();
        return ret;
    }
    else
    {
        printf("trying to start parsing a non int value \n");
        exit(1);
        return nullptr;
    }
}