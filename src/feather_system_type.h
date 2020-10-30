#pragma once
#include "feather_variable.h"

namespace fsl
{

    class int_type : public variable_type
    {
        int value;

    public:
        virtual uint64_t get_value() override
        {
            return value;
        }
        virtual void init(void *base_value) override;
        virtual void set(variable_type *v) override;

        virtual const char *get_type_name() override
        {
            return "int";
        };
        virtual feather_variable_type get_type() override
        {
            return VAR_TYPE_INT;
        };
        virtual variable_type *convert(feather_variable_type to) override;

        virtual variable_type *add(variable_type *a) override;
        virtual variable_type *sub(variable_type *a) override;
        virtual variable_type *mul(variable_type *a) override;
        virtual variable_type *div(variable_type *a) override;
    };
    class float_type : public variable_type
    {
        float value;
    };
    class string_type : public variable_type
    {
        const char *value;
    };
    class char_type : public int_type
    {
    };
    class bool_type : public int_type
    {
    };
    class address_type : public int_type
    {
    };
    class custom_type : public variable_type
    {
        // implemented later
    };
} // namespace fsl
