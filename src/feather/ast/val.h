
#pragma once 

#include <stdbool.h>

#define FOREACH_CONST_AST(VACTION) \
    VACTION(CONST_STRING) \
    VACTION(CONST_NAME) \
    VACTION(CONST_SIGNED) \
    VACTION(CONST_UNSIGNED) \
    VACTION(CONST_FLOAT) \
    VACTION(CONST_BOOL) \
    VACTION(CONST_NULL)


typedef union
{
    const char* str;
    const char* identifier;
    long signed_;
    unsigned long unsigned_;
    float floating_point;
    bool null;
} FthVal;
