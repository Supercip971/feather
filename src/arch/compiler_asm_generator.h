#ifndef D55FF439_F67E_46DF_85E6_E5BF39A8BE5A
#define D55FF439_F67E_46DF_85E6_E5BF39A8BE5A
#include <string>
class asm_generator{
    protected:
    std::string str;
    
    public:
    virtual void asm_start() = 0;
    virtual void asm_end() = 0;
    
    void init_output();
    void close_output_in_file(const char* out);
    
    virtual int alloc_register() = 0;
    virtual void free_register(int reg) = 0;

    virtual int gen_add(int left_register, int right_register) = 0;
    virtual int gen_sub(int left_register, int right_register) = 0;
    virtual int gen_mul(int left_register, int right_register) = 0;
    virtual int gen_div(int left_register, int right_register) = 0;
    
    virtual int gen_load(int value) = 0;
};

#endif /* D55FF439_F67E_46DF_85E6_E5BF39A8BE5A */
