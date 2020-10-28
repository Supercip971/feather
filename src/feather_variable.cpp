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
    feather_variable::feather_variable(int64_t val, const char *name, feather_variable_type var_type)
    {

        variable_name = (char *)malloc(strlen(name) + 1);
        memcpy(variable_name, name, strlen(name) + 1);
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
            printf("var list already is created \n");
            destroy();
        }
        var_list_length = 1;
        var_list = (feather_variable *)malloc(sizeof(feather_variable) * (var_list_length));
    }

    void feather_variable_list::dump()
    {
        for (uint64_t i = 0; i < var_list_length + 1; i++)
        {

            if (var_list[i].get_var_name() == nullptr)
            {
                continue;
            }

            printf("%s = %i \n", var_list[i].get_var_name(), var_list[i].get_value());
        }
    }
    void feather_variable_list::add_variable(uint64_t value, const char *name, feather_variable_type type)
    {
        char *copy = to_valid_var_name(name);

        if (var_list == nullptr)
        {
            printf("creating var list \n");
            create();
        }
        if (find_variable(name) != nullptr)
        {
            printf("variable %s already exist", name);
            return;
        }
        var_list_length++;
        var_list = (feather_variable *)realloc(var_list, sizeof(feather_variable) * (var_list_length));

        var_list[var_list_length - 1] = feather_variable(value, copy, type);
        // printf("added var %i = %x, %s, of type %i \n",var_list_length-1,  var_list[var_list_length-1].get_value(),  var_list[var_list_length-1].get_var_name(),  var_list[var_list_length-1].get_type());
    }

    char *feather_variable_list::to_valid_var_name(const char *name)
    {
        char *copy = (char *)malloc(sizeof(char) * strlen(name) + 1);
        memcpy(copy, name, strlen(name) + 1);
        int length = 0;
        for (size_t i = 0; i < strlen(name); i++)
        {
            if (copy[i] == ' ')
            {
                for (size_t j = i; j < strlen(name); j++)
                {
                    copy[j] = copy[j + 1];
                }
            }
            else
            {
                length++;
            }
        }
        copy[length] = 0;
        return copy;
    }
    feather_variable *feather_variable_list::find_variable(const char *name)
    {
        char *n = to_valid_var_name(name);

        for (uint64_t i = 0; i < var_list_length + 1; i++)
        {

            if (var_list[i].get_var_name() == nullptr)
            {
                continue;
            }
            if (strcmp(n, var_list[i].get_var_name()) == 0)
            {
                free(n);
                return &var_list[i];
            }
        }
        printf("variable with name %s not founded \n", name);
        return nullptr;
    }

} // namespace fsl
