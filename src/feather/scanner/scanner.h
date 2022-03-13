#pragma once 

#include <stddef.h>
#include <feather/utils/str.h>
#include <stdbool.h>

typedef struct 
{
    size_t size;
    char* buffer;

    int cur;
} Scanner;


typedef int fscan_char_fn(int c);

void fscan_init(Scanner* self, void* data, size_t size);

void fscan_deinit(Scanner* self);

bool fscan_skip(Scanner* self, Str str);

Str fscan_skip_until(Scanner* self, fscan_char_fn* fn);

int fscan_cursor(Scanner* self);

char fscan_current(Scanner* self);

bool fscan_ended(Scanner* self);

char fscan_next(Scanner* self);
