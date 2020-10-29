#pragma once

#include "feather_lexer.h"
#include "feather_type.h"
#include "feather_variable.h"
#include "feather_vector.h"
#include <stdint.h>
namespace fsl
{
    struct function_argument{
        const char* name;
        feather_variable_type type;
    };

    class feather_function
    {

        feather_lexer_entry *dady;
        const char* name;
        uint64_t return_type;
        uint64_t start;
        uint64_t end;
        uint64_t line;
        feather_vector<function_argument> arguments;
    public:
        feather_function();
        void init_argument(feather_lexer_entry *d, uint64_t entry);
        feather_function(feather_lexer_entry *d, uint64_t entry);
        void initerpret_call_argument(feather_variable_list* var_list, feather_lexer_entry * entry_list);

        uint64_t get_start();
        uint64_t get_line();
        uint64_t get_end();
        const char* get_name();
        uint64_t get_return();
    };

} // namespace fsl
