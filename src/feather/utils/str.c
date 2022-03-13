#include <feather/utils/str.h>

bool str_eq(Str const lhs,  Str const rhs)
{
    if(lhs.len != rhs.len)
    {
        return false;
    }

    for(size_t i  = 0; i < lhs.len; i++)
    {
        if(lhs.buf[i] != rhs.buf[i])
        {
            return false;
        }
    }

    return true;
}
