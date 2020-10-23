
#pragma once
#include "feather_vm.h"
#include "feather_lexer.h"
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

    uint64_t feather_virtual_machine::interpret_multiple_entry(feather_lexer_entry *entry, uint64_t count)
    {
        for(uint64_t i = 0; i< count; i++){
            if(entry[i].type == TYPE_TOKEN){
                if(entry[i+1].type == TYPE_DELIMITOR && entry[i+2].type == TYPE_DELIMITOR){
                    feather_lexer_entry  * function = find_function_definition(entry[i].data);
                    if(function == nullptr){
                        return -1;
                    }
                    run_code(find_function_start(*function));
                    programm_counter.pop();
                }
            } // to thing
              // if there is a ';' after it is a variable definition
              // if there is a '=' after it is a variable assignation
              // if there is a '(' after it is a function
        }
        return 0;
    }

    void feather_virtual_machine::run_code(uint64_t from)
    {
        printf("running code from %i \n", from);
        uint64_t current_line_entry = 0;
        feather_lexer_entry *current_line = new feather_lexer_entry[64]; // max 64 entry in a 'line'
        uint64_t current_context = 1;
        programm_counter.push(from);
        while (current_context != 0)
        {

            printf("running %i / %s \n", programm_counter.current(), lexer_info->entry[programm_counter.current()].data);

            current_line[current_line_entry] = lexer_info->entry[programm_counter.current()];
            current_line_entry++;

            programm_counter.current()++;
            uint64_t next = programm_counter.current();
            if (lexer_info->entry[next].type == TYPE_END_OF_LINE)
            {
                interpret_multiple_entry(current_line, current_line_entry);
                current_line_entry = 0;

            }else if(lexer_info->entry[next].type == TYPE_DELIMITOR){
                if(lexer_info->entry[next].subtype==DELIMITOR_CODE_BLOCK_OPEN){
                    current_context++;
                }else if(lexer_info->entry[next].subtype ==DELIMITOR_CODE_BLOCK_CLOSE){
                    current_context--;
                    if(current_context == 0){

                        return;
                    }
                }
            }
          //  printf("next %i / %s \n", programm_counter.current(), lexer_info->entry[programm_counter.current()].data);

        }
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
} // namespace fsl
