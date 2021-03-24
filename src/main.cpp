#include "arch/x86_64_compiler_asm.h"
#include "parser.h"
#include "scanner.h"
#include <fstream>
#include <stdio.h>
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("must past file, feather [file] ");
        return -1;
    }
    printf("using file %s \n", argv[1]);

    scanner scn;
    scn.open(argv[1]);

    parser par = parser(scn);
    asm_generator *generator = new x86_64_asm_generator();

    auto v = par.create_ast_node();

    generator->init_output();

    generator->gen_debug_print(v->generate(generator));

    generator->close_output_in_file("output.s");

    v->destroy();
    delete generator;
    return 0;
}
