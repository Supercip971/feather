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
        VAR_TYPE_CUSTOM, // for class
    };

    class variable_type
    {
        const char *type_name;
        feather_variable_type type;
        char *variable_name = nullptr;

    public:
        virtual void init(void *base_value) = 0; // yeah that's a bad idea
        virtual uint64_t get_value() = 0;
        virtual void set(variable_type *v) = 0;
        virtual const char *get_type_name() = 0;
        virtual variable_type *convert(feather_variable_type to) = 0;

        virtual feather_variable_type get_type()
        {
            return (feather_variable_type)0;
        }
        virtual variable_type *add(variable_type *a) = 0;
        virtual variable_type *sub(variable_type *a) = 0;
        virtual variable_type *mul(variable_type *a) = 0;
        virtual variable_type *div(variable_type *a) = 0;
    };
    class feather_variable
    {
        char *variable_name = nullptr;
        bool garbage = true;
        variable_type *storage;
        feather_variable_type type;

    public:
        feather_variable();
        feather_variable(int64_t val, const char *name, feather_variable_type var_type);

        constexpr inline variable_type *get_storage()
        {
            return storage;
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

        char *to_valid_var_name(const char *name);

    public:
        feather_variable_list();

        inline static feather_variable_list *get_global_context_list()
        {
            return &global_list;
        }
        void dump();
        void destroy();
        void create();
        void add_variable(uint64_t value, const char *name, feather_variable_type type);
        feather_variable *find_variable(const char *name);
    };

} // namespace fsl
