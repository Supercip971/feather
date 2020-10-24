#include "feather_variable.h"

namespace fsl
{

    feather_variable::feather_variable()
    {
        garbage = true;
        value = false;
        variable_name = nullptr;
        type = VAR_TYPE_ADDRESS;
        value = 0;
    }
    feather_variable::feather_variable(uint64_t val, const char *name, feather_variable_type var_type)
    {
        variable_name = name;
        value = val;
        type = var_type;
    }

    feather_variable_list::feather_variable_list()
    {
        var_list = nullptr;
        var_list_length = 0;
    }

    void feather_variable_list::destroy()
    {
        if (var_list != nullptr)
        {
            free(var_list);
        }
    }
    void feather_variable_list::create()
    {
        if (var_list != nullptr)
        {
            destroy();
        }
        var_list_length = 1;
        var_list = (feather_variable *)malloc(sizeof(feather_variable) * (var_list_length));
    }
    void feather_variable_list::add_variable(uint64_t value, const char *name, feather_variable_type type)
    {
        if (var_list == nullptr)
        {
            create();
        }
        if (find_variable(name) != nullptr)
        {
            printf("variable %s already exist", name);
            return;
        }
        var_list = (feather_variable *)realloc(var_list, sizeof(feather_variable) * (var_list_length++));
        var_list[var_list_length - 1] = feather_variable(value, name, type);
        printf("added var %x, %s, of type %i \n", value, name, type);
    }
    feather_variable *feather_variable_list::find_variable(const char *name)
    {
        for (uint64_t i = 0; i < var_list_length; i++)
        {
            if (var_list[i].get_var_name() == nullptr)
            {
                continue;
            }
            if (strcmp(name, var_list[i].get_var_name()))
            {
                return &var_list[i];
            }
        }

        return nullptr;
    }

} // namespace fsl
