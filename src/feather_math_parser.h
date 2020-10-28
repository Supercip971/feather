#pragma once
#include "feather_lexer.h"
#include "feather_vector.h"
#include "feather_vm.h"
#include <stdint.h>
namespace fsl
{
    struct math_entry
    {
        int64_t value;      // i think everything is in the name
        uint64_t type;      // if it is a value or an operator
        uint64_t operators; // + -
    };
    class feather_math_expression
    {
        feather_vector<math_entry> math_vector;
        void interpret_part(uint64_t operator_is_at); // note : it remove the
        uint64_t interpret_region(uint64_t start, uint64_t length);

    public:
        uint64_t interpret(feather_lexer_entry *entry, uint64_t count, uint64_t end_statement, feather_virtual_machine* target);
    };
} // namespace fsl
