#pragma once

#include <bits/stdint-uintn.h>
#include <stdint.h>
namespace fsl
{
    struct feather_lexer_entry
    {
        uint64_t position;
        uint64_t line;
        uint64_t type;
        uint64_t subtype;
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
    };
} // namespace fsl