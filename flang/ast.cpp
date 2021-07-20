//
// Created by root on 5/11/21.
//

#include "ast.h"

std::string ast_type_to_str(ast_types type)
{
    switch (type)
    {

    case AST_NULL:
        return "null";

    case AST_ADD:
        return "add";
    case AST_SUBSTRACT:
        return "substract";
    case AST_MULTIPLY:
        return "multiply";
    case AST_DIVIDE:
        return "divide";
    case AST_EQUAL:
        return "equal";

    case AST_PARENTHESIS_START:
        return "parenthesis start";

    case AST_PARENTHESIS_END:
        return "parenthesis end";

    case AST_END_OF_LINE:
        return "end of line";

    case AST_INT_VALUE:
        return "int value";
    case AST_KEYWORD:
        return "keyword";
    case AST_VARIABLE_DEF:
        return "variable definition";
    case AST_FUNCTION_DEF:
        return "function definition";
    case AST_FUNCTION_TYPE:
        return "function type";
    case AST_FUNCTION_ARGUMENT:
        return "function argument";
    case AST_VAR_INSTANCE:
        return "variable instance";
    case AST_CURLY_START:
        return "curly start";
    case AST_CURLY_END:
        return "curly end";
    case AST_CODE:
        return "ast code";
    case AST_FUNC_CALL:
        return "function call";
    case AST_COMMA:
        return "comma";
    case AST_IF:
        return "if";
    case AST_ELSEIF:
        return "elif";
    case AST_ELSE:
        return "else";
    default:
        return "not detected type";
    }
}

ast_types ast_type_from_token(token tok)
{
    switch (tok.type())
    {
    case TOKEN_NULL:
        return ast_types::AST_NULL;
    case PLUS:
        return ast_types::AST_ADD;
    case MINUS:
        return ast_types::AST_SUBSTRACT;
    case STAR:
        return ast_types::AST_MULTIPLY;
    case SLASH:
        return ast_types::AST_DIVIDE;
    case SEMICOLON:
        return ast_types::AST_END_OF_LINE;
    case INT_VALUE:
        return ast_types::AST_INT_VALUE;
    case PARENTHESIS_OPEN:
        return ast_types::AST_PARENTHESIS_START;
    case PARENTHESIS_CLOSE:
        return ast_types::AST_PARENTHESIS_END;
    case END_OF_STREAM:
        return ast_types::AST_END_OF_LINE;
    case KEYWORD:
        return ast_types::AST_KEYWORD;
    case EQUAL:
        return ast_types::AST_EQUAL;
    case CURLY_START:
        return ast_types::AST_CURLY_START;
    case CURLY_END:
        return ast_types::AST_CURLY_END;
    case COMMA:
        return ast_types::AST_COMMA;
    default:
        return ast_types::AST_NULL;
    }
}

int precedence_from_ast_type(ast_types type)
{
    switch (type)
    {
    case ast_types::AST_NULL:
    case ast_types::AST_INT_VALUE:
    case ast_types::AST_END_OF_LINE:
        return 0;

    case ast_types::AST_ADD:
    case ast_types::AST_SUBSTRACT:
        return 10;

    case ast_types::AST_MULTIPLY:
    case ast_types::AST_DIVIDE:
        return 20;
    case ast_types::AST_PARENTHESIS_END:
    case ast_types::AST_PARENTHESIS_START:
        return 0;

    case ast_types::AST_CURLY_END:
    case ast_types::AST_CURLY_START:
        return 0;

    case ast_types::AST_KEYWORD:
        return 30;
    case ast_types::AST_EQUAL:
        return 30;
    case ast_types::AST_COMMA:
        return 30;
    default:
        printf("invalide precedence from ast type: %i \n", type);
        exit(-1);
        return 0;
    }
}
ast_node ast_node::create_from_token(class token tok)
{
    return ast_node(tok, ast_type_from_token(tok));
}

void ast_node::debug_out_recursive(int depth)
{

    for (int i = 0; i < depth; i++)
    {
        printf("\t");
    }
    if (type() == AST_KEYWORD || type() == AST_FUNCTION_DEF || type() == AST_FUNC_CALL)
    {

        printf("ast node: type %s (%s)\n", ast_type_to_str(_type).c_str(),
               _token.raw().c_str());
    }
    else
    {

        printf("ast node: type %s \n", ast_type_to_str(_type).c_str());
    }

    for (int i = 0; i < _sub_nodes.size(); i++)
    {
        _sub_nodes[i].debug_out_recursive(depth + 1);
    }
}

size_t ast_node::interpret()
{
    std::vector<size_t> results;
    for (int i = 0; i < _sub_nodes.size(); i++)
    {
        results.push_back(_sub_nodes[i].interpret());
    }

    switch (type())
    {

    case ast_types::AST_NULL:
        printf("null ast type");
        return -1;

    case ast_types::AST_INT_VALUE:
        return stoi(_token.raw());

    case ast_types::AST_ADD:
        return results[0] + results[1];

    case ast_types::AST_SUBSTRACT:
        return results[0] - results[1];

    case ast_types::AST_MULTIPLY:
        return results[0] * results[1];
    case ast_types::AST_DIVIDE:
        return results[0] / results[1];
    case ast_types::AST_VARIABLE_DEF:
        printf("creating variable %s", token().raw().c_str());
        printf(" = %lu \n", results[0]);
        return 0;

    default:
        return 0;
    }
    return 0;
}
