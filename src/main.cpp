#include <stdint.h>
#include "scanner.h"
#include "cstar_token.h"
#include "parser.h"
int main(int argc, char** argv)
{
    scanner scan = scanner();
    scan.open("test.cst");

    parser pars = parser(scan);
    auto nodes = pars.create_ast_node();

    nodes.debug_out_recursive(0);

    printf("result: %zu \n", nodes.interpret());

    return 0;
}