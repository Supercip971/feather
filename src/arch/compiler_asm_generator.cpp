#include "compiler_asm_generator.h"
#include <fstream>
#include <iostream>
void asm_generator::init_output()
{
    stream = std::ofstream("./output.s");
    stream.clear();
    asm_start();
}
void asm_generator::close_output_in_file(const char *out)
{
    asm_end();
    stream.close();
}