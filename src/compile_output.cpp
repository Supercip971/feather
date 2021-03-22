#include "compile_output.h"

struct compiler_register{
    const char* name;
    bool status;
};

compiler_register reg_table[] = {
    {"%r8", false},
    {"%r9", false},
    {"%r10", false}, 
    {"%r11", false},
};

#define REG_TABLE_SIZE (sizeof(reg_table)/sizeof(compiler_register))
void output_compiler::init_output(){
    str = "\t.text\n"
	".LC0:\n"
	"\t.string\t\"%d\\n\"\n"
	"printint:\n"
	"\tpushq\t%rbp\n"
	"\tmovq\t%rsp, %rbp\n"
	"\tsubq\t$16, %rsp\n"
	"\tmovl\t%edi, -4(%rbp)\n"
	"\tmovl\t-4(%rbp), %eax\n"
	"\tmovl\t%eax, %esi\n"
	"\tleaq	.LC0(%rip), %rdi\n"
	"\tmovl	$0, %eax\n"
	"\tcall	printf@PLT\n"
	"\tnop\n"
	"\tleave\n"
	"\tret\n"
	"\n"
	"\t.globl\tmain\n"
	"\t.type\tmain, @function\n"
	"main:\n"
	"\tpushq\t%rbp\n"
	"\tmovq	%rsp, %rbp\n";
}
void output_compiler::close_output_in_file(const char* out){
    str += 
    "\tmovl	$0, %eax\n"
	"\tpopq	%rbp\n"
	"\tret\n";
    printf("output asm: \n %s \n", str.c_str());

}
int output_compiler::alloc_register(){
    for(int i = 0; i < REG_TABLE_SIZE; i++){
        if(!reg_table[i].status){
            reg_table[i].status = true;
            return i;
        }
    }
    printf("can't find free register \n");
    exit(1);
}
void output_compiler::free_register(int reg){
    if(!reg_table[reg].status){
        printf("error trying to free an already free register \n");
        exit(1);
    }
    reg_table[reg].status = false;
}