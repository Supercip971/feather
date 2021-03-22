#include "x86_64_compiler_asm.h"
#include <string.h>
#include <stdio.h>
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

void x86_64_asm_generator::gen_two_reg_operator(int reg1, int reg2){
    stream << reg_table[reg1].name << ", " << reg_table[reg2].name << "\n";
}
void x86_64_asm_generator::asm_start(){
    stream << "\t.text\n"
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
void x86_64_asm_generator::asm_end(){
    stream << 
    "\tmovl	$0, %eax\n"
	"\tpopq	%rbp\n"
	"\tret\n";

}
int x86_64_asm_generator::alloc_register(){
    for(int i = 0; i < REG_TABLE_SIZE; i++){
        if(!reg_table[i].status){
            reg_table[i].status = true;
            return i;
        }
    }
    printf("can't find free register \n");
    exit(1);
}
void x86_64_asm_generator::free_register(int reg){
    if(!reg_table[reg].status){
        printf("error trying to free an already free register \n");
        exit(1);
    }
    reg_table[reg].status = false;
}


int x86_64_asm_generator::gen_add(int left_register, int right_register){
    gen_instruction("addq");
    gen_two_reg_operator(left_register, right_register);
    free_register(left_register);
    return right_register;
};
int x86_64_asm_generator::gen_sub(int left_register, int right_register){
    return 0;

};
int x86_64_asm_generator::gen_mul(int left_register, int right_register) {


    gen_instruction("imulq");
    gen_two_reg_operator(left_register, right_register);
    free_register(left_register);
    return right_register;
};
int x86_64_asm_generator::gen_div(int left_register, int right_register){

    gen_mov( "%rax", reg_table[left_register].name);
    gen_single_line_instruction("cqo");
    gen_one_arg_instruction("idivq", reg_table[right_register].name);

    gen_mov(reg_table[left_register].name,  "%rax");
    free_register(right_register);
    return left_register;
};
    
int x86_64_asm_generator::gen_load(int value){
    int reg = alloc_register();

    // waiting for the c++20 format library support
    stream << "\tmovq\t$" << std::to_string(value) << ", " << reg_table[reg].name << "\n";

    return reg;
};
int x86_64_asm_generator::gen_debug_print(int reg){

    gen_mov( "%rdi", reg_table[reg].name);
    gen_one_arg_instruction("call", "printint");
    free_register(reg);
    return 0;
}