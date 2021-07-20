//
// Created by root on 5/11/21.
//

#include "parser.h"
ast_node parser::math_expression(size_t lprecedence, std::set<token_type> end)
{
    ast_node left, right;
    ast_types type;

    left = start();

    if (current().type() == token_type::END_OF_STREAM || end.contains(current().type()))
    {
        return left;
    }

    type = ast_node::create_from_token(current()).type();

    while (precedence_from_ast_type(type) > lprecedence)
    {
        _current_token = _scan.next_token();

        right = math_expression(precedence_from_ast_type(type), end);

        if (right.type() == AST_PARENTHESIS_START)
        {
            right = math_expression(precedence_from_ast_type(AST_PARENTHESIS_END), end);
        }
        else if (right.type() == AST_KEYWORD && current().type() == PARENTHESIS_OPEN)
        {
            next_node(); // skip func[(])
            right = parse_function_call(right);
        }

        auto upper_node = ast_node::create_from_token(current());

        upper_node.type(type);
        upper_node.add_node(left);
        upper_node.add_node(right);
        left = upper_node;

        type = ast_node::create_from_token(current()).type();

        if (type == ast_types::AST_NULL || type == ast_types::AST_END_OF_LINE || end.contains(current().type()))
        {
            return left;
        }
    }

    return left;
}
ast_node parser::create_ast_node()
{

    _current_token = _scan.next_token();
    return expression();
}

ast_node parser::parse_argument_declaration()
{
    ast_node final = ast_node(ast_types::AST_FUNCTION_ARGUMENT);
    while (current().type() != token_type::END_OF_STREAM && current().type() != token_type::PARENTHESIS_CLOSE)
    {
        ast_node current_arg = ast_node(ast_types::AST_VAR_INSTANCE);

        current_arg.add_node(ast_node(current(), ast_types::AST_KEYWORD));
        next_node();
        current_arg.add_node(ast_node(current(), ast_types::AST_KEYWORD));
        next_node();

        if (current().type() == token_type::PARENTHESIS_CLOSE)
        {
            final.add_node(current_arg);
            next_node();
            return final;
        }
        else if (current().type() == token_type::COMMA)
        {
            final.add_node(current_arg);
            next_node();
        }
        else
        {
            printf("expected ',' or ')' after argument: token");
            current().debug_print();
            exit(1);
        }
    }
    return final;
}

ast_node parser::parse_code()
{
    ast_node final_code = ast_node(ast_types::AST_CODE);

    while (true)
    {
        ast_node to_add = expression();
        if (to_add.token().type() == token_type::CURLY_END)
        {
            return final_code;
        }
        else if (to_add.type() == ast_types::AST_NULL || to_add.token().type() == token_type::END_OF_STREAM)
        {
            printf("expected a valid ast type in code block \n");
            current().debug_print();
            exit(1);
        }
        else
        {
            final_code.add_node(to_add);
        }
    }
    return final_code;
}
ast_node parser::parse_function_declaration(ast_node return_type, ast_node name)
{
    ast_node final_node = ast_node(name.token(), ast_types::AST_FUNCTION_DEF);
    ast_node type = ast_node(return_type.token(), ast_types::AST_FUNCTION_TYPE);
    type.add_node(parse_argument_declaration());
    type.add_node(return_type);
    final_node.add_node(type);

    if (current().type() == token_type::CURLY_START)
    {
        next_node();
        final_node.add_node(parse_code());
    }
    return final_node;
}

ast_node parser::parse_condition_block(ast_node block_type)
{

    ast_node final_node;

    if (block_type.token().raw() == "if")
    {
        final_node = ast_node(block_type.token(), ast_types::AST_IF);
    }
    else if (block_type.token().raw() == "elif")
    {
        final_node = ast_node(block_type.token(), ast_types::AST_ELSEIF);
    }
    else if (block_type.token().raw() == "else")
    {
        final_node = ast_node(block_type.token(), ast_types::AST_ELSE);
    }
    ast_node arg = math_expression(precedence_from_ast_type(ast_types::AST_PARENTHESIS_END), {token_type::PARENTHESIS_CLOSE});

    final_node.add_node(arg);
    next_node();

    if (current().type() == token_type::CURLY_START)
    {
        next_node();
        final_node.add_node(parse_code());
    }
    else
    {

        printf("expected '{' after condition \n");
        current().debug_print();
        exit(1);
    }

    return final_node;
}

ast_node parser::parse_function_call(ast_node func_name)
{
    ast_node final_node = ast_node(func_name.token(), ast_types::AST_FUNC_CALL);

    while (current().type() != token_type::PARENTHESIS_CLOSE)
    {

        ast_node arg = math_expression(precedence_from_ast_type(ast_types::AST_PARENTHESIS_END), {token_type::COMMA});
        // parse argument call
        final_node.add_node(arg);
        if (current().type() == token_type::END_OF_STREAM)
        {

            printf("expected ')' in function call parameters \n");
            current().debug_print();
            exit(1);
        }
        else if (current().type() == token_type::PARENTHESIS_CLOSE)
        {
            next_node();
            return final_node;
        }
        next_node();
    }
    next_node();

    return final_node;
}

bool is_str_a_cond_block_keyword(const std::string &str)
{
    if (str == "if")
    {
        return true;
    }
    if (str == "elif")
    {
        return true;
    }
    if (str == "else")
    {
        return true;
    }
    return false;
}

ast_node parser::expression()
{

    auto left = start();
    ast_node final;
    if (left.type() == AST_KEYWORD)
    {
        printf("parsing variable/function definition \n");

        auto second = next_node();

        if (second.type() == AST_PARENTHESIS_START && is_str_a_cond_block_keyword(left.token().raw()))
        {
            return parse_condition_block(left);
        }
        else if (second.type() == AST_PARENTHESIS_START)
        {
            // function call
            return parse_function_call(left);
        }
        auto third = next_node();

        if (third.type() == AST_END_OF_LINE || third.type() == AST_EQUAL)
        {
            final = ast_node(second.token(), ast_types::AST_VARIABLE_DEF);
            if (third.type() != AST_END_OF_LINE)
            {
                final.add_node(math_expression(0, {}));
            }
        }

        else if (third.type() == AST_PARENTHESIS_START)
        {
            return parse_function_declaration(left, second);
            // function definition
        }
        else
        {
            printf("unexpected keyword type: %s \n", left.token().raw().c_str());
            current().debug_print();
            exit(1);
        }
    }
    else
    {
        return left;
    }
    return final;
}
ast_node parser::next_node()
{
    auto ret = ast_node::create_from_token(current());
    _current_token = _scan.next_token();
    return ret;
}

ast_node parser::start()
{

    auto ret = ast_node::create_from_token(current());
    _current_token = _scan.next_token();
    return ret;
}

token parser::expect_next(token_type type)
{
    if (_current_token.type() != type)
    {
        printf("error token:");
        _current_token.debug_print();
        printf("expected: %i \n", type);
    }
    _current_token = _scan.next_token();
    return _current_token;
}
