#include "feather_math_parser.h"
#include <stdlib.h>
namespace fsl
{
    void feather_math_expression::interpret_part(uint64_t operator_is_at)
    {

        uint64_t result = 0;

        math_entry *right = math_vector[operator_is_at + 1];
        math_entry *left = math_vector[operator_is_at - 1];

        uint64_t operator_subtype = math_vector[operator_is_at]->operators;

        if (operator_subtype == OPERATOR_ADD)
        {
            result = right->value + left->value;
        }
        else if (operator_subtype == OPERATOR_MULTIPLY)
        {
            result = right->value * left->value;
        }
        else if (operator_subtype == OPERATOR_SUBSTRACT)
        {
            result = left->value - right->value;
        }
        math_entry new_entry;
        new_entry.type = TYPE_NUMBER;
        new_entry.value = result;

        uint64_t target_to_insert = operator_is_at - 1;
        printf("\n");
        printf("\n");
        for (int i = 0; i < 3; i++)
        {
            printf("\n");
            for (int j = 0; j < 10; j++)
            {

                if (math_vector[j]->type == TYPE_NUMBER)
                {

                    printf("[%i][%i] %i \n", operator_is_at, j, math_vector[j]->value);
                }
                else if (math_vector[j]->type == TYPE_OPERATOR)
                {

                    printf("[%i][%i] %s \n", operator_is_at, j, feather_operator_list.find_from_co((feather_operator)math_vector[j]->operators));
                }
            }
            math_vector.remove_entry(target_to_insert);
        }
        math_vector.insert_entry(target_to_insert, new_entry);
    }
    uint64_t feather_math_expression::interpret_region(uint64_t start, uint64_t length)
    {
        uint64_t r = 0;
        int current_level_number = 0;
        for (int current_level = 0; current_level < 2; current_level++)
        {
            current_level_number = 0;
            for (uint64_t i = start; i < length + start; i++)
            {
                math_entry *target = math_vector[i];
                if (target->type != TYPE_OPERATOR)
                {
                    continue;
                }
                if ((target->operators == OPERATOR_MULTIPLY || target->operators == OPERATOR_DIVIDE) && current_level == 0)
                {
                    current_level_number++;
                    interpret_part(i);
                }
                else
                {
                    if (current_level != 1)
                    {
                        continue;
                    }
                    if (target->operators == OPERATOR_ADD || target->operators == OPERATOR_SUBSTRACT)
                    {
                        current_level_number++;
                        interpret_part(i);
                    }
                }
            }
            printf("\n === \n");
            for (int j = 0; j < 10; j++)
            {

                if (math_vector[j]->type == TYPE_NUMBER)
                {

                    printf("[%i][%i] %x \n", current_level, j, math_vector[j]->value);
                }
                else if (math_vector[j]->type == TYPE_OPERATOR)
                {

                    printf("[%i][%i] %s \n", current_level, j, feather_operator_list.find_from_co((feather_operator)math_vector[j]->operators));
                }
            }
            printf("\n === \n");
            if (current_level_number != 0)
            {
                current_level_number = 0;
                current_level--;
            }
        }
        return math_vector[0]->value;
    }
    uint64_t feather_math_expression::interpret(feather_lexer_entry *entry, uint64_t count, uint64_t end_statement)
    {
        math_vector.create();
        bool has_delimitor = false;
        for (uint64_t i = count; entry[i].type != end_statement; i++)
        {
            math_entry current = {0};
            if (entry[i].type == TYPE_OPERATOR || entry[i].type == TYPE_DELIMITOR)
            {
                if (entry[i].type == TYPE_DELIMITOR)
                {
                    printf("'()' are not supported for the moment :^( \n");
                    has_delimitor = true;
                    continue;
                }
                current.type = entry[i].type;
                current.operators = entry[i].subtype;
                math_vector.push(current);
            }
            else if (entry[i].type == TYPE_NUMBER)
            {
                current.type = TYPE_NUMBER;
                current.value = atoi(entry[i].data);
                math_vector.push(current);
            }
            else
            {
                printf("invalid math expression entry %s \n", entry[i].data);
            }
        }
        if (!has_delimitor)
        {

            return interpret_region(0, math_vector.get_length());
        }
        return 0;
    }
} // namespace fsl
