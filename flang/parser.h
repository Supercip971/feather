//
// Created by root on 5/11/21.
//

#ifndef FLANG_PARSER_H
#define FLANG_PARSER_H

#include "ast.h"
#include "scanner.h"
#include <set>

class parser
{
    scanner _scan;
    token _current_token;
    std::vector<ast_node> node_list;
    ast_node start();
    ast_node math_expression(size_t predecendence, std::set<token_type> end);
    ast_node expression();
    ast_node next_node();
    ast_node parse_code();

    ast_node parse_condition_block(ast_node block_type);
    ast_node parse_argument_declaration();
    ast_node parse_function_declaration(ast_node return_type, ast_node name);
    ast_node parse_function_call(ast_node name);
    token expect_next(token_type type);

public:
    parser(scanner scan) : _scan(scan){};
    token current() const
    {
        return _current_token;
    }

    ast_node create_ast_node();
};

#endif //FLANG_PARSER_H
