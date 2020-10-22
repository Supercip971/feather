#pragma once
#include "feather_lexer.h"
#include "feather_type.h"
#include <bits/stdint-uintn.h>
#include <sys/types.h>
namespace fsl
{

    class feather_context
    {
        uint64_t child_count;
        feather_list<feather_context> child;
        feather_context *parent;
        feather_list<feather_h_element> element;
        uint64_t start_line = 0;
        perline_information *plifo;
        feather_lexer *lex;
        uint64_t context_offset_start;

    public:
        feather_h_element *find_element(const char *element_name);
        // parent_context = nullptr when it is the main context
        feather_context();
        feather_context(perline_information *perline_info, uint64_t context_line_start, feather_context *parent_context, feather_lexer *lexer);
        void init_element();
        void run();
    };
    class feather_vm
    {
        char *data_copy = nullptr;
        feather_lexer current_lexer;
        uint64_t programm_length;
        perline_information *pli_info;
        feather_context main_context;

    public:
        feather_vm();
        void run();
        void init(const char *data);
    };

} // namespace fsl