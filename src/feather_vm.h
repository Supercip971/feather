
#pragma once
#include "feather_lexer.h"
#include "feather_type.h"
namespace fsl
{
    class feather_programm_counter
    {
        static const int max_programm_counter = 64;
        uint64_t get_current = 0;
        uint64_t line_counter[max_programm_counter];

    public:
        inline constexpr uint64_t &current()
        {
            return line_counter[get_current];
        }

        inline constexpr uint64_t &push(uint64_t value)
        {
            get_current++;
            line_counter[get_current] = value;
            return line_counter[get_current];
        }
        inline constexpr uint64_t pop()
        {
            line_counter[get_current] = 0;
            get_current--;
            return line_counter[get_current];
        }
    };

    class feather_virtual_machine
    {
        fsl::lexer main_lexer;
        feather_lexer_info *lexer_info;
        feather_programm_counter programm_counter;
        uint64_t find_function_start(feather_lexer_entry function); // for later pre definition too
        feather_lexer_entry *find_function_definition(const char *name);

        void run_code(uint64_t from);
        uint64_t interpret_multiple_entry(feather_lexer_entry *entry, uint64_t count);

    public:
        feather_virtual_machine();
        feather_virtual_machine(const char *code_data);
        void start();
    };
} // namespace fsl
