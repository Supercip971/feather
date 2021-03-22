#include "compiler_asm_generator.h"

void asm_generator::init_output(){
    str = "";
    asm_start();
}
void asm_generator::close_output_in_file(const char* out){
    asm_end();
    printf("output asm: \n %s \n", str.c_str());

}