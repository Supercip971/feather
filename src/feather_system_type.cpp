#include "feather_system_type.h"

namespace fsl
{

    void int_type::init(void *base_value)
    {

        value = *((int *)base_value);
    }

    void int_type::set(variable_type *v)
    {

        variable_type *d = v->convert(feather_variable_type::VAR_TYPE_INT);
        value = d->get_value();
    }

    variable_type *int_type::convert(feather_variable_type to)
    {
        if (to == feather_variable_type::VAR_TYPE_INT)
        {
            return this;
        }
        else
        {
            printf("not supported conversion to type id %i \n", (int)to);
        }
        return this;
    };

    variable_type *int_type::add(variable_type *a)
    {
        variable_type *d = a->convert(feather_variable_type::VAR_TYPE_INT);
        value += d->get_value();
        return this;
    };
    variable_type *int_type::sub(variable_type *a)
    {
        variable_type *d = a->convert(feather_variable_type::VAR_TYPE_INT);
        value -= d->get_value();
        return this;
    };
    variable_type *int_type::mul(variable_type *a)
    {
        variable_type *d = a->convert(feather_variable_type::VAR_TYPE_INT);
        value *= d->get_value();
        return this;
    }
    variable_type *int_type::div(variable_type *a)
    {
        variable_type *d = a->convert(feather_variable_type::VAR_TYPE_INT);
        value /= d->get_value();
        return this;
    }

} // namespace fsl
