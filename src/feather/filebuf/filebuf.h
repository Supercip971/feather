#pragma once 

#include <stddef.h>
#include <feather/utils/str.h>
#include <stdbool.h>

typedef struct 
{
    size_t size;
    char* buffer;

    int cur;
} FileBuf;


typedef int fbuf_char_fn(int c);

void fbuf_init(FileBuf* self, void* data, size_t size);

void fbuf_deinit(FileBuf* self);

bool fbuf_skip(FileBuf* self, Str str);

Str fbuf_skip_until(FileBuf* self, fbuf_char_fn* fn);

int fbuf_cursor(FileBuf* self);

char fbuf_current(FileBuf* self);

bool fbuf_ended(FileBuf* self);

char fbuf_next(FileBuf* self);
