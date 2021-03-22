#ifndef E0572878_4A59_44A2_8977_0030FED6177E
#define E0572878_4A59_44A2_8977_0030FED6177E
#include "compiler_asm_generator.h"


class x86_64_asm_generator : public asm_generator{
    
    public:
    virtual void asm_start() override;
    virtual void asm_end() override;
    
    virtual int alloc_register() override;
    virtual void free_register(int reg) override;

    virtual int gen_add(int left_register, int right_register) override;
    virtual int gen_sub(int left_register, int right_register) override;
    virtual int gen_mul(int left_register, int right_register) override;
    virtual int gen_div(int left_register, int right_register) override;
    
    virtual int gen_load(int value) override;
};
#endif /* E0572878_4A59_44A2_8977_0030FED6177E */
