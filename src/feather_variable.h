#pragma once

#pragma once
#include "feather_lexer.h"
#include "feather_type.h"
namespace fsl
{

    enum feather_variable_type
    {
        VAR_TYPE_FUNC = 1,
        VAR_TYPE_BOOL,
        VAR_TYPE_INT,
        VAR_TYPE_FLOAT,
        VAR_TYPE_STRING,
        VAR_TYPE_CHAR,
        VAR_TYPE_ADDRESS,
    };

    class feather_variable
    {
        char *variable_name = nullptr;
        bool garbage = true;
        uint64_t value;
        feather_variable_type type;

    public:
        feather_variable();
        feather_variable(uint64_t val, const char *name, feather_variable_type var_type);

        constexpr inline uint64_t get_value()
        {
            return value;
        }

        constexpr inline void set_value(uint64_t val)
        {
            value = val;
        }

        constexpr inline feather_variable_type get_type()
        {
            return type;
        }

        constexpr inline const char *get_var_name()
        {
            return variable_name;
        }
    };

    class feather_variable_list
    {
        uint64_t var_list_length = 0;
        feather_variable *var_list = nullptr;
        static feather_variable_list global_list; // may be used for global system variable
        bool loaded = false;                      // like feather_time / feather_version ...
    public:
        feather_variable_list();

        inline static feather_variable_list *get_global_context_list()
        {
            return &global_list;
        }
        void destroy();
        void create();
        void add_variable(uint64_t value, const char *name, feather_variable_type type);
        feather_variable *find_variable(const char *name);
    };
} // namespace fsl
