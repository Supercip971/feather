#pragma once

#include "feather_lexer.h"
#include "feather_type.h"
#include <stdint.h>
namespace fsl
{

    class feather_function
    {

        feather_lexer_entry *dady;
        const char* name;
        uint64_t return_type;
        uint64_t start;
        uint64_t end;
        uint64_t line;
    public:
        feather_function();
        feather_function(feather_lexer_entry *d, uint64_t entry);
        uint64_t get_start();
        uint64_t get_line();
        uint64_t get_end();
        const char* get_name();
        uint64_t get_return();
    };

} // namespace fsl
