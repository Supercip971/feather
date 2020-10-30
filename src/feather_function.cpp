#include "feather_function.h"

namespace fsl
{

    feather_function::feather_function()
    {
        start = 0;
        end = 0;
        dady = nullptr;
    }
    void feather_function::init_argument(feather_lexer_entry *d, uint64_t entry)
    {
        arguments.create();
        while (true)
        {

            if (d[start].type == TYPE_SPECIFIC && d[start].subtype < NAME_IF)
            {
                if (d[start + 1].type == TYPE_TOKEN)
                {
                    if (d[start + 2].type == TYPE_DELIMITOR && d[start + 2].subtype == DELIMITOR_ARGUMENT_BLOCK_CLOSE)
                    {
                        printf("argument %s of type %s \n", d[start + 1].data, d[start].data);
                        arguments.push({d[start + 1].data, (feather_variable_type)d[start].subtype});
                        break;
                    }
                    else if (d[start + 2].type == TYPE_SPECIFIC && d[start + 2].subtype == NAME_LIST_DELIMIT)
                    {
                        printf("argument %s of type %s \n", d[start + 1].data, d[start].data);
                        arguments.push({d[start + 1].data, (feather_variable_type)d[start].subtype});
                    }
                    else
                    {
                        printf("invalid element %s \n", d[start + 2].data);
                    }
                }
                else
                {
                    printf("invalid function declaration\n");
                }
            }
            if (d[start].type == TYPE_DELIMITOR && d[start].subtype == DELIMITOR_ARGUMENT_BLOCK_CLOSE)
            {
                break;
            }
            start++;
        }
    }
    feather_function::feather_function(feather_lexer_entry *d, uint64_t entry)
    {
        if (d[entry].type != TYPE_SPECIFIC)
        {
            printf("not valid function creation\n");
            return;
        }
        if (d[entry].subtype != NAME_FUNC)
        {
            printf("not valid function creation\n");
            return;
        }
        special = false;
        name = d[entry + 1].data;
        line = d[entry].line;
        printf("creating function named %s \n", name);
        start = entry;
        return_type = 0;
        init_argument(d, entry);

        while (true)
        {
            if (d[start].type == TYPE_DELIMITOR && d[start].subtype == DELIMITOR_CODE_BLOCK_OPEN)
            {
                break;
            }
            if (d[start].type == TYPE_SPECIFIC)
            {
                if (d[start].subtype == NAME_RETURN_TYPE)
                {
                    if (d[start + 1].type != TYPE_SPECIFIC)
                    {
                        printf("not valid function creation\n");
                        // invalid
                        break;
                    }

                    printf("return type is %i \n ", d[start + 1].subtype);
                    return_type = d[start + 1].subtype;
                }
            }
            start++;
        }
    }
    feather_function::feather_function(const char *special_name, void (*callback)(feather_vector<function_argument> arg))
    {
        name = special_name;
        line = 0;
        special = true;
        tocallback = callback;
    }
    bool feather_function::set_valid_argument(feather_vector<function_argument> &target_arg_list)
    {
        if (target_arg_list.get_length() != arguments.get_length())
        {
            printf("not valid length %i where it should be %i \n", target_arg_list.get_length(), arguments.get_length());
            return false;
        }
        for (int i = 0; i < arguments.get_length(); i++)
        {
            if (target_arg_list[i]->type != arguments[i]->type)
            {
                printf("invalid type for %s \n", arguments[i]->name);
                return false;
            }
            else
            {
                target_arg_list[i]->name = arguments[i]->name;
            }
        }
        return true;
    }
    uint64_t feather_function::get_start()
    {
        return start;
    }
    uint64_t feather_function::get_end()
    {
        return 0; // not implemented
    }

    uint64_t feather_function::get_return()
    {
        return return_type;
    }

    uint64_t feather_function::get_line()
    {
        return line;
    }
    const char *feather_function::get_name()
    {
        return name;
    }

} // namespace fsl
