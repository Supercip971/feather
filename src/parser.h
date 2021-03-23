#ifndef PARSER_H
#define PARSER_H
#include "defs.h"
#include "scanner.h"
class parser
{
    scanner current_scanner;
    token current_token;
    ast_node *start();
    ast_node *expression(int lprecedence);

public:
    parser(scanner scanner) : current_scanner(scanner), current_token(token_type::NULL_TOKEN){};
    ast_node *create_ast_node();
};

#endif // PARSER_H
