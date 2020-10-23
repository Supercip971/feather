#pragma once

#include "feather_type.h"
#include <bits/stdint-uintn.h>
#include <stdint.h>
namespace fsl
{
    struct feather_lexer_entry
    {
        char* data;
        uint64_t position;
        uint64_t line;
        feather_main_element_type type;
        uint64_t subtype;
    };
    struct feather_lexer_info
    {
        feather_lexer_entry *entry;
        uint64_t entry_count;
    };
    class lexer
    {
        char *data_copy;
        uint64_t data_length;
        char *next_word(uint64_t *current_position);
        void continue_until_valid(uint64_t *pos);
        /* 
         next word include some little thing that strtok has not
         it jump ' ' but if you have ({[]}) or any operator (= + / ...) it will just give the element 1 time
         so for exemple : 
         hello world(1+1)
         1 hello
         2 world
         3 (
         4 ) 
         5 1
         6 +
         7 1
        */
    public:
        lexer();
        lexer(const char *code);
        bool is_an_operator(const char *data);
        bool is_an_specific_item(const char *data);
        bool is_an_delimitor(const char *data);
        feather_lexer_info *generate_information();
    };
} // namespace fsl