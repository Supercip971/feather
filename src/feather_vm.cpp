
#pragma once
#include "feather_vm.h"
#include "feather_lexer.h"
#include "feather_math_parser.h"
#include "feather_type.h"
namespace fsl
{

    feather_virtual_machine::feather_virtual_machine()
    {
    }

    feather_virtual_machine::feather_virtual_machine(const char *code_data)
    {
        printf("loading feather virtual machine\n");
        printf("loading lexer \n");

        main_lexer = lexer(code_data);
        lexer_info = main_lexer.get_info();
    }

    uint64_t feather_virtual_machine::interpret_subcode(feather_lexer_entry *entry, uint64_t count, uint64_t end_statement)
    {
        feather_math_expression expression = feather_math_expression();

        return expression.interpret(entry, count, end_statement);
    }
    uint64_t feather_virtual_machine::interpret_line_specific(feather_lexer_entry *entry, uint64_t entry_id)
    {
        if (entry[entry_id].subtype < NAME_IF)
        { // func / type so it is not an if or any other thing

            if (entry[entry_id + 1].type != TYPE_TOKEN)
            {
                printf("invalid next type after entry %s, it should be a token \n", entry[entry_id + 1].data);
            }
            printf("creating variable of type %s named %s \n", entry[entry_id].data, entry[entry_id + 1].data);
            feather_variable_list &list = programm_counter.current_var_list();

            list.add_variable(0, entry[entry_id + 1].data, (feather_variable_type)entry[entry_id].subtype);

            if (entry[entry_id + 2].type != TYPE_OPERATOR)
            {
                if (entry[entry_id + 2].type == TYPE_END_OF_LINE)
                {
                    return 0;
                }
                else
                {
                    printf("invalid entry after variable definition \n");
                }
            }
            else
            {
                uint64_t r = interpret_subcode(entry, entry_id + 3, TYPE_END_OF_LINE); //
                printf("var value %i \n", r);
                list.find_variable(entry[entry_id + 1].data)->set_value(r);
            }
            return 0;
        }

        return 0;
    }
    uint64_t feather_virtual_machine::interpret_multiple_entry(feather_lexer_entry *entry, uint64_t count)
    {
        for (uint64_t i = 0; i < count; i++)
        {
            if (entry[i].type == TYPE_TOKEN)
            {
                if (entry[i + 1].type == TYPE_DELIMITOR && entry[i + 2].type == TYPE_DELIMITOR)
                {
                    feather_lexer_entry *function = find_function_definition(entry[i].data);
                    if (function == nullptr)
                    {
                        return -1;
                    }
                    uint64_t result = run_code(find_function_start(*function));
                    programm_counter.pop();
                }
            } // to thing
              // if there is a ';' after it is a variable definition
              // if there is a '=' after it is a variable assignation
              // if there is a '(' after it is a function call
            else if (entry[i].type == TYPE_SPECIFIC && entry[i].subtype != NAME_FUNC)
            { // specific can only be on the first word of a line
                if (i != 0)
                {
                    printf("specific entry isn't at the start of the line %i (%s) / %i\n", entry[i].line, entry[i].data,i);
                    printf("previous entry %s \n", entry[i-1].data);
                }
                interpret_line_specific(entry, i);
            }
        }
        return 0;
    }

    uint64_t feather_virtual_machine::run_code(uint64_t from)
    {
        printf("running code from %i \n", from);
        uint64_t current_line_entry = 0;
        feather_lexer_entry *current_line = new feather_lexer_entry[64]; // max 64 entry in a 'line'
        uint64_t current_context = 1;
        programm_counter.push(from);
        uint64_t last_result = 0;
        while (current_context != 0)
        {
            printf("running %i / %s \n", programm_counter.current(), lexer_info->entry[programm_counter.current()].data);

            //printf("running %i / %s / %i \n", programm_counter.current(), lexer_info->entry[programm_counter.current()].data, current_line_entry);
            if(current_line_entry != 0 || lexer_info->entry[programm_counter.current()].type != TYPE_END_OF_LINE){
                current_line[current_line_entry] = lexer_info->entry[programm_counter.current()];

                current_line_entry++;

            }
            programm_counter.current()++;
            uint64_t next = programm_counter.current();
            if (lexer_info->entry[next].type == TYPE_END_OF_LINE)
            {
                current_line[current_line_entry++] = lexer_info->entry[programm_counter.current()];

                last_result = interpret_multiple_entry(current_line, current_line_entry);
                current_line_entry = 0;
            }
            else if (lexer_info->entry[next].type == TYPE_DELIMITOR)
            {
                if (lexer_info->entry[next].subtype == DELIMITOR_CODE_BLOCK_OPEN)
                {
                    current_context++;
                }
                else if (lexer_info->entry[next].subtype == DELIMITOR_CODE_BLOCK_CLOSE)
                {
                    current_context--;
                    if (current_context == 0)
                    {

                        return last_result;
                    }
                }
            }
            //  printf("next %i / %s \n", programm_counter.current(), lexer_info->entry[programm_counter.current()].data);
        }
        return 0;
    }

    void feather_virtual_machine::start()
    {
        printf("starting feather virtual machine \n");

        programm_counter.current() = 0;
        feather_lexer_entry *main_entry = find_function_definition("main");
        if (main_entry == nullptr)
        {
            printf("no main function founded  \n");
            return;
        }
        printf("main function founded at line %x \n", main_entry->line);
        programm_counter.current_var_list().create();
        run_code(find_function_start(*main_entry));
    }

    uint64_t feather_virtual_machine::find_function_start(feather_lexer_entry function)
    {
        uint64_t current = function.entry_id;
        while (true)
        {
            printf("checking %x \n", current);
            feather_lexer_entry *is_this_the_one = &lexer_info->entry[current];
            if (is_this_the_one->type == TYPE_DELIMITOR)
            {
                if (is_this_the_one->subtype == DELIMITOR_CODE_BLOCK_OPEN)
                {
                    return is_this_the_one->entry_id + 1; // this is the { so we skip to the next entry
                }
            }
            current++;
        }
    }

    feather_lexer_entry *feather_virtual_machine::find_function_definition(const char *name)
    {
        bool verify_next = false;
        for (uint64_t i = 0; i < lexer_info->entry_count; i++)
        {
            feather_lexer_entry *entry = &lexer_info->entry[i];
            if (verify_next == true)
            {
                if (entry->type != TYPE_TOKEN)
                {
                    printf("invalid entry type next a func declaration on line %i \n", entry->line);
                    return nullptr;
                }
                if (strcmp(entry->data, name) == 0)
                {
                    return entry;
                }
                verify_next = false;
            }
            if (entry->type == TYPE_SPECIFIC && entry->subtype == NAME_FUNC)
            { // that mean that the next should be the function name

                verify_next = true;
            }
        }
        return nullptr;
    }

    feather_variable* feather_virtual_machine::find_variable_value(const char* name){
        return programm_counter.current_var_list().find_variable(name);
    }
} // namespace fsl
