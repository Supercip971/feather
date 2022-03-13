#include <string.h>
#include <feather/filebuf/filebuf.h>

void fbuf_init(FileBuf* self, void* data, size_t size)
{
    self->buffer = data;
    self->cur = 0;
    self->size = size;
}

void fbuf_deinit(FileBuf* self)
{
    (void)self;
}

bool fbuf_skip(FileBuf* self, Str str)
{
    for (size_t i = 0; i < str.len; i++)
    {
        if(self->buffer[self->cur + i] != str.buf[i]
            || fbuf_ended(self))
        {
            return false;
        }
    }

    self->cur += str.len;

    return true;
}

Str fbuf_skip_until(FileBuf* self, fbuf_char_fn* fn)
{
    size_t begin = self->cur;

    while(fn(self->buffer[self->cur]) && !fbuf_ended(self))
    {
        self->cur++;
    }

    return (Str) {
        .buf = self->buffer + begin,
        .len = self->cur - begin,
    };
}

int fbuf_cur(FileBuf* self)
{
    return self->cur;
}

bool fbuf_ended(FileBuf* self)
{
    return (size_t)self->cur >= (self->size);
}

char fbuf_current(FileBuf* self)
{
    return self->buffer[self->cur];
}

char fbuf_next(FileBuf* self)
{
    self->cur++;
    return self->buffer[self->cur - 1];
}
