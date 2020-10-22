#include "feather_lexer.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
namespace fsl
{

    void feather_lexer::fill_lines()
    {

        uint64_t cur_line_count = get_line_count();
        uint64_t cur_line_cursor = 0;
        uint64_t cur_line_length = 0;
        for (int i = 0; i < cur_line_count; i++)
        {

            for (int j = 0; j < 100; j++)
            {
                cur_line_length++;
                if (copy[j + cur_line_cursor] == endline_code)
                {
                    break;
                }
            }
            lines[i] = new char[cur_line_length + 3];
            memcpy(lines[i], copy + cur_line_cursor, cur_line_length);

            cur_line_cursor += cur_line_length;
            cur_line_length = 0;
            printf("detected line : %s", lines[i]);
        }
    }
    uint64_t feather_lexer::get_word_count() { return 0; }

    void feather_lexer::init_elements()
    {
        uint64_t element_to_init = 512;
        items = new feather_lexer_item[element_to_init];
        elements = new feather_element_item[element_to_init];
        item_c = 0;
        element_c = 0;
        uint64_t cur_item;

        const char *operator_list = "=*+-/%";
        uint64_t cur_line_count = get_line_count();
        uint64_t j = 0;
        bool has_space_beetween = false;
        bool is_a_good_boy = false;
        for (uint64_t i = 0; i < cur_line_count; i++)
        {
            j = 0;

            while (lines[i][j] != '\n')
            {

                is_a_good_boy = false;
                has_space_beetween = true;

                if (j != 0)
                {

                    if (lines[i][j - 1] != ' ')
                    {
                        has_space_beetween = false;
                    }
                }
                int current_id = 0;
                for (uint64_t k = 0; k < sizeof(sys_type) / sizeof(char *); k++)
                {
                    uint64_t stlength = strlen(sys_type[k]);
                    if (stlength == 0)
                    {
                        current_id++;
                        continue;
                    }
                    if (strncmp((char *)((uint64_t)lines[i] + j), sys_type[k], stlength) ==
                        0)
                    {
                        if (lines[i][j + stlength] != ' ')
                        {
                            has_space_beetween = false;
                        }
                        if (current_id != 1 && current_id != 2)
                        {
                            if (!has_space_beetween)
                            {
                                break;
                            }
                        }
                        items[item_c].line = i;
                        items[item_c].position = j;
                        items[item_c].type = current_id + 1;

                        items[item_c].subtype = k;
                        item_c++;
                        is_a_good_boy = true;
                        break;
                    }
                }
                if (!is_a_good_boy)
                {
                    bool valid = true;
                    if (j != 0)
                    {

                        if (lines[i][j - 1] != ' ' && lines[i][j - 1] != '"')
                        {
                            valid = false;
                        }
                    }
                    if (valid)
                    {
                        elements[element_c].position = j;
                        elements[element_c].line = i;
                        uint32_t length = 0;
                        for (length = 0; isalpha(lines[i][j + length]); length++)
                        {
                        }
                        if (length == 0)
                        {
                            j++;
                            continue;
                        }
                        elements[element_c].name = (char *)malloc(length);
                        memcpy(elements[element_c].name, lines[i] + j, length);
                        element_c++;
                    }
                }

                j++;
            }
        }

        for (int i = 0; i < item_c; i++)
        {
            printf("A line = %i / pos = %i / t = %i / %s \n", items[i].line,
                   items[i].position, items[i].subtype, sys_type[items[i].subtype]);
        }
        for (int i = 0; i < element_c; i++)
        {
            printf("B line = %i / pos = %i / %s \n", elements[i].line,
                   elements[i].position, elements[i].name);
        }
    }
    bool feather_lexer::is_open_delimitor(feather_lexer_item item)
    {
        if (sys_type[item.subtype][0] == '(')
        {
            return true;
        }
        else if (sys_type[item.subtype][0] == '{')
        {
            return true;
        }
        else if (sys_type[item.subtype][0] == '[')
        {
            return true;
        }

        return false;
    }
    void feather_lexer::list_context()
    {
        uint64_t current_context_count = 0;
        for (int i = 0; i < item_c; i++)
        {
            if (items[i].type == FEATHER_DELIMITOR)
            {

                if (is_open_delimitor(items[i]))
                {
                    current_context_count++;
                }
                else
                {
                    current_context_count--;
                }
            }
            printf("%i | %s \n", current_context_count, sys_type[items[i].subtype]);
        }
    }

    uint32_t feather_lexer::get_context_at(uint64_t line)
    {
       
        
        uint64_t current_context_count = 0;
        for (int i = 0; i < item_c; i++)
        {
            if (items[i].line > line)
            {
                return current_context_count;
            }
            if (sys_type[items[i].subtype][0] == '{' || sys_type[items[i].subtype][0] == '}')
            {

                if (is_open_delimitor(items[i]))
                {
                    current_context_count++;
                }
                else
                {
                    current_context_count--;
                }
            }
        }

        return current_context_count;
    }
    uint32_t feather_lexer::get_context_at(uint64_t line, uint64_t position)
    {
       
        
        uint64_t current_context_count = 0;
        for (int i = 0; i < item_c; i++)
        {
            if (items[i].line == line && items[i].position > position)
            {
                return current_context_count;
            }
            if (items[i].line > line)
            {
                return current_context_count;
            }
            if (sys_type[items[i].subtype][0] == '{' || sys_type[items[i].subtype][0] == '}')
            {

                if (is_open_delimitor(items[i]))
                {
                    current_context_count++;
                }
                else
                {
                    current_context_count--;
                }
            }
        }

        return current_context_count;
    }
    perline_information *feather_lexer::generate_information()
    {

        uint64_t cur_line_count = get_line_count();
        perline_information *pli = new perline_information[cur_line_count + 2];
        for (int i = 0; i < cur_line_count; i++)
        {
            pli[i].current_line = i;
            pli[i].length = strlen(lines[i]);
            pli[i].line_start_context = get_context_at(i);
            uint64_t d = 0;
            for (int j = 0; j < item_c; j++)
            {
                if (items[j].line == i)
                {
                    pli[i].litem[d] = &items[j];
                    d++;
                }
            }

            pli[i].litem_length = d;
            d = 0;

            for (int j = 0; j < element_c; j++)
            {
                if (elements[j].line == i)
                {
                    pli[i].felment[d] = &elements[j];
                    d++;
                }
            }
            pli[i].felment_length = d;
        }
        pli[cur_line_count + 1].current_line = -1;
        return pli;
    }
    void feather_lexer::parse(char *data)
    {
        printf("starting lexer : %s \n", data);
        length = strlen(data);
        copy = data;
        uint64_t cur_line_count = get_line_count();
        lines = new char *[cur_line_count];
        uint64_t temp_cursor = 0;
        fill_lines();
        init_elements();
        list_context();
    };
    uint64_t feather_lexer::get_line_count()
    {
        uint64_t line_count = 0;
        for (uint64_t i = 0; i < length; i++)
        {
            if (copy[i] == endline_code)
            {
                line_count++;
            }
        }
        return line_count;
    }
} // namespace fsl
// thank janco_atoka freeloop and Meaxy for the programming language name !
// feather <3
