
#pragma once
// namespace name : fsl
// Feather Scripting Language
// here is some sample programm :

/**
 * sample code for this :
 *  import console;
 *  int sample_function(string a){
 *      ret printl(a);
 *  };
 *  noret main(){
 *      sample_function("hello world");
 *  };
 *
 */
#include "feather_type.h"
#include <stdint.h>
namespace fsl
{

    class feather_lexer
    {

        static const char endline_code = '\n';
        uint64_t length;
        char **lines;
        feather_lexer_item *items;
        feather_element_item *elements;
        uint64_t item_c = 0;
        uint64_t element_c = 0;
        char *copy;
        void fill_lines();
        void read_line_data();
        uint64_t get_word_count();
        void init_elements();
        void list_context();
        bool is_open_delimitor(feather_lexer_item item);

    public:
        void parse(char *data);
        perline_information *generate_information();
        uint32_t get_context_at(uint64_t line);
        uint32_t get_context_at(uint64_t line, uint64_t position);
        uint64_t get_line_count();
    };

} // namespace fsl
