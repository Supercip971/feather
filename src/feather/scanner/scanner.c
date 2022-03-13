#include <string.h>
#include <feather/scanner/scanner.h>

void fscan_init(Scanner* self, void* data, size_t size)
{
    self->buffer = data;
    self->cur = 0;
    self->size = size;
}

void fscan_deinit(Scanner* self)
{
    (void)self;
}

bool fscan_skip(Scanner* self, Str str)
{
    for (size_t i = 0; i < str.len; i++)
    {
        if(self->buffer[self->cur + i] != str.buf[i]
            || fscan_ended(self))
        {
            return false;
        }
    }

    self->cur += str.len;

    return true;
}

Str fscan_skip_until(Scanner* self, fscan_char_fn* fn)
{
    size_t begin = self->cur;

    while(fn(self->buffer[self->cur]) && !fscan_ended(self))
    {
        self->cur++;
    }

    return (Str) {
        .buf = self->buffer + begin,
        .len = self->cur - begin,
    };
}

int fscan_cur(Scanner* self)
{
    return self->cur;
}

bool fscan_ended(Scanner* self)
{
    return (size_t)self->cur >= (self->size);
}

char fscan_current(Scanner* self)
{
    return self->buffer[self->cur];
}

char fscan_next(Scanner* self)
{
    self->cur++;
    return self->buffer[self->cur - 1];
}
