#pragma once
#include "feather_lexer.h"
#include "feather_type.h"
namespace fsl
{
    class feather_vm
    {
        char *data_copy = nullptr;
        feather_lexer current_lexer;
        uint64_t programm_length;

    public:
        void init(const char *data);
    };

} // namespace fsl