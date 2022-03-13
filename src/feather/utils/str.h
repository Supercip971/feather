#pragma once 

#include <stddef.h>
#include <stdbool.h>

// Taken from brutal (we should one day put it in a separated repo)
typedef struct 
{
    size_t len;
    const char* buf;
} Str;

bool str_eq(Str const lhs, Str const rhs);

#define str$(literal) \
    (Str){.len = strlen(literal), .buf = (literal)}

#define str_const$(literal) \
    (Str){.len = (sizeof(literal)) - 1, .buf = (literal)}



#define str_n$(size, literal) \
    (Str){.len = (size), .buf = (literal)}

#define str_sub(str, start, end) \
    str_n$((end) - (start), (char *)(str).buf + (start))
