#include "feather_function.h"

namespace fsl
{

    feather_function::feather_function()
    {
        start = 0;
        end = 0;
        dady = nullptr;
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
        name = d[entry+1].data;
        line = d[entry].line;
        printf("creating function named %s \n", name);
        start = entry;
        return_type  =0;
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
                    if(d[start + 1].type != TYPE_SPECIFIC){
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

    uint64_t feather_function::get_line(){
        return line;
    }
    const char* feather_function::get_name(){
        return name;
    }

} // namespace fsl
