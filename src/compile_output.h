#ifndef E484CDEA_25C8_4C63_A8C7_8622467CF62A
#define E484CDEA_25C8_4C63_A8C7_8622467CF62A
#include <string>
class output_compiler{
    std::string str;
    public:
    void init_output();
    void close_output_in_file(const char* out);
    int alloc_register();
    void free_register(int reg);
    int add(int left_register, int right_register);
};

#endif /* E484CDEA_25C8_4C63_A8C7_8622467CF62A */
