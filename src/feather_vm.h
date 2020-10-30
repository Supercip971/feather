
#pragma once
#include "feather_function.h"
#include "feather_lexer.h"
#include "feather_type.h"
#include "feather_variable.h"
#include "feather_vector.h"
namespace fsl
{

    class feather_programm_counter
    {
        static const int max_programm_counter = 64;
        uint64_t main_current = 0;
        uint64_t get_current = 0;
        feather_variable_list context_list[max_programm_counter];
        uint64_t line_counter[max_programm_counter];

    public:
        inline constexpr feather_variable_list &current_var_list()
        {
            return context_list[get_current];
        }
        inline constexpr uint64_t &current()
        {
            return line_counter[get_current];
        }
        inline uint64_t &push(uint64_t value)
        {
            get_current++;
            context_list[get_current].destroy();
            context_list[get_current] = feather_variable_list();
            context_list[get_current].create();
            line_counter[get_current] = value;
            return line_counter[get_current];
        }
        inline uint64_t pop()
        {
            context_list[get_current].destroy();
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
        void vmprintf(feather_vector<function_argument> arg);
        feather_vector<function_argument> *generate_argument_list(feather_lexer_entry *entry, uint64_t count, uint64_t start /* start for '('*/);
        feather_vector<feather_function> function_list;
        uint64_t find_function_start(feather_function function); // for later pre definition too
        feather_function *find_function_definition(const char *name);
        void init_function_list();
        uint64_t run_code_without_pushing_context(uint64_t from);
        uint64_t run_code(uint64_t from);
        // uint64_t interpret_sub_sub_code(feather_lexer_entry *entry, uint64_t start, uint64_t end); // used for element in '(' ')'
        uint64_t interpret_subcode(feather_lexer_entry *entry, uint64_t count, uint64_t end_statement, uint64_t endstatement_subtype); // used for variable definition or if/elseif/while statement
        uint64_t interpret_multiple_entry(feather_lexer_entry *entry, uint64_t count);
        uint64_t interpret_line_specific(feather_lexer_entry *entry, uint64_t entry_id);

    public:
        feather_variable *find_variable_value(const char *name);
        feather_virtual_machine();
        feather_virtual_machine(const char *code_data);
        void start();
    };
} // namespace fsl
