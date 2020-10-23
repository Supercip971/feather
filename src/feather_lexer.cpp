#include "feather_lexer.h"
#include "feather_type.h"
#include <bits/stdint-uintn.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
namespace fsl
{
    bool lexer_feather_map_verification(const char *a, const char *b)
    {
        return (strncmp(a, b, strlen(a)) == 0);
    }

    void lexer::continue_until_valid(uint64_t *pos)
    {
        while (data_copy[*pos] == ' ' || data_copy[*pos] == '\n')
        {
            (*pos)++;
            if (*pos == data_length)
            {
                return;
            }
        };
    }
    char *lexer::next_word(uint64_t *current_position)
    {

        if (*current_position > data_length)
        {
            return nullptr;
        }
        continue_until_valid(current_position);
        if (*current_position > data_length)
        {
            return nullptr;
        }

        uint64_t current_length = 0;
        bool op = false;
        bool eol = false;
        bool del = false;
        uint64_t start_pos = *current_position;

        while (true)
        {

            current_length++;

            if (*current_position == data_length)
            {
                return nullptr;
            }
            char current_char = data_copy[*current_position];
            if (current_char == ' ' || current_char == '\n')
            {
                (*current_position)++;
                break;
            }
            if (data_copy[*current_position] == ';') // line break
            {
                eol = true;
                if (current_length != 1)
                {
                    current_length--;
                }
                break;
            }
            if (is_an_operator(data_copy + *current_position))
            {
                op = true;
                if (current_length != 1)
                {
                    current_length--;
                }
                break;
            }
            if (is_an_delimitor(data_copy + *current_position))
            {
                del = true;
                if (current_length != 1) // in the case of blabla( reduce length of 1
                {
                    current_length--;
                }
                break;
            }
            (*current_position)++;
        }

        char *d = (char *)malloc(current_length + 1);
        memset(d, 0, current_length + 1);
        strncpy(d, data_copy + start_pos, current_length);

        if ((del || op || eol) && current_length == 1)
        {
            (*current_position)++;
        }
        return d;
    };
    uint64_t lexer::line_position_from_index(uint64_t index)
    {
        uint64_t cur_line = 0;
        for (int i = 0; i < index; i++)
        {
            if (data_copy[i] == '\n')
            {
                cur_line++;
            }
        }
        return cur_line;
    }

    lexer::lexer()
    {
    }
    lexer::lexer(const char *code)
    {
        data_copy = new char[strlen(code) + 1];
        strcpy(data_copy, code);
        data_length = strlen(data_copy);
        uint64_t test = 0;
        char *result = next_word(&test);
        while (result != nullptr)
        {
            printf("%i | %s \n", test, result);
            free(result);
            result = next_word(&test);
        }
        feather_lexer_info *info = generate_information();
        uint64_t last_line = 0;
        for (int i = 0; i < info->entry_count; i++)
        {

            feather_lexer_entry *entry_to_print = &info->entry[i];
            if (entry_to_print->line != last_line)
            {
                last_line = entry_to_print->line;
                printf(" [===] at line %i \n", last_line);
            }
            if (entry_to_print->type == TYPE_TOKEN)
            {
                printf("[token] %s \n", entry_to_print->data);
            }
            else if (entry_to_print->type == TYPE_DELIMITOR)
            {
                printf("[delimitor] %s \n", entry_to_print->data);
            }
            else if (entry_to_print->type == TYPE_SPECIFIC)
            {
                printf("[specific] %s \n", entry_to_print->data);
            }
            else if (entry_to_print->type == TYPE_OPERATOR)
            {
                printf("[operator] %s \n", entry_to_print->data);
            }
            else if (entry_to_print->type == TYPE_END_OF_LINE)
            {
                printf("[end of line] %s\n", entry_to_print->data);
            }
            else if (entry_to_print->type == TYPE_NUMBER)
            {
                printf("[number] %s\n", entry_to_print->data);
            }
        }
        global_info = info;
    }
    bool lexer::is_only_number(char *dat)
    {
        uint64_t length_to_check = strlen(dat);
        for (int i = 0; i < length_to_check; i++)
        {
            if (dat[i] >= '0' && dat[i] <= '9')
            {
                continue;
            }
            if (dat[i] == ' ' || dat[i] == '.' || dat[i] == '\n')
            {
                continue;
            }
            return false;
        }
        return true;
    }
    feather_lexer_info *lexer::generate_information()
    {
        uint64_t current_idex = 0;
        uint64_t max_word_count = 512;
        uint64_t current_entry = 0;
        uint64_t last_idex = 0;
        feather_lexer_info *information = new feather_lexer_info;
        information->entry = new feather_lexer_entry[max_word_count];

        char *result = next_word(&current_idex);
        while (result != nullptr)
        {
            feather_lexer_entry *entry_to_edit = &information->entry[current_entry];
            entry_to_edit->position = current_idex;
            entry_to_edit->entry_id = current_entry;
            entry_to_edit->line = line_position_from_index(current_idex);
            if (is_an_operator(result))
            {
                entry_to_edit->subtype = feather_operator_list.find_from_id_verification(result, lexer_feather_map_verification); // later we will check for func or other declaration

                entry_to_edit->type = TYPE_OPERATOR;
            }
            else if (is_an_specific_item(result))
            {
                entry_to_edit->type = TYPE_SPECIFIC;
                entry_to_edit->subtype = feather_specific_item_list.find_from_id_verification(result, lexer_feather_map_verification); // later we will check for func or other declaration
            }
            else if (is_an_delimitor(result))
            {
                entry_to_edit->type = TYPE_DELIMITOR;

                entry_to_edit->subtype = feather_delimitor_list.find_from_id(result[0]); // later we will check for func or other declaration
            }
            else if (result[0] == ';')
            {
                entry_to_edit->type = TYPE_END_OF_LINE;

                entry_to_edit->subtype = 0; // later we will check for func or other declaration
            }
            else
            {
                if (is_only_number(result))
                {
                    entry_to_edit->type = TYPE_NUMBER;
                }
                else
                {
                    entry_to_edit->type = TYPE_TOKEN;
                }

                entry_to_edit->subtype = 0; // later we will check for func or other declaration
            }
            entry_to_edit->data = result;
            current_entry++;
            last_idex = current_idex;
            result = next_word(&current_idex);
        }
        information->entry_count = current_entry;
        return information;
    }

    bool lexer::is_an_operator(const char *data)
    {
        int result = feather_operator_list.find_from_id_verification(data, lexer_feather_map_verification);
        return (result != 0);
    }
    bool lexer::is_an_specific_item(const char *data)
    {
        int result = feather_specific_item_list.find_from_id_verification(data, lexer_feather_map_verification);
        return (result != 0);
    }
    bool lexer::is_an_delimitor(const char *data)
    {
        int result = feather_delimitor_list.find_from_id(data[0]);
        return (result != 0);
    }
} // namespace fsl
