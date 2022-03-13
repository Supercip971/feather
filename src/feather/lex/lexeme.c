#include <ctype.h>
#include <feather/lex/lexeme.h>
#include <feather/utils/log.h>

#define LEXEME_FOREACH_TABLE(entry, val) [LEX_##entry] = str_const$(val),

Str lexeme_table[] = {LEXEME_FOREACH(LEXEME_FOREACH_TABLE)};

static LexemeType parse_lexeme_type(FileBuf *buf)
{

    if (isspace(buf->buffer[buf->cur]))
    {
        fbuf_skip_until(buf, &isspace);
        return LEX_WHITESPACE;
    }

    for (size_t i = 0; i < sizeof(lexeme_table) / sizeof(lexeme_table[0]); i++)
    {
        Str entry = lexeme_table[i];
        Str rhs = str_n$(entry.len, buf->buffer + buf->cur);
        if (entry.len == 0)
        {
            // not handled by us
            continue;
        }
        else if (entry.len < buf->size &&
                 str_eq(entry, rhs))
        {
            buf->cur += entry.len;
            return (LexemeType)i;
        }
    }

    if (isalpha(fbuf_current(buf)))
    {
        fbuf_skip_until(buf, &isalnum);

        return LEX_IDENT;
    }

    if (isdigit(fbuf_current(buf)))
    {
        fbuf_skip_until(buf, &isdigit);

        return LEX_INTEGER;
    }

    if (fbuf_current(buf) == '"')
    {
        fbuf_next(buf);
        while (fbuf_next(buf) != '"' && !fbuf_ended(buf))
        {
        }
        return LEX_STRING;
    }

    return LEX_NONE;
}

bool parse_lexemes(Lexemes *self, FileBuf *buf)
{
    vec_init(&self->lexemes);
    vec_init(&self->str_list);

    while (!fbuf_ended(buf))
    {
        int beg = buf->cur;
        LexemeType type = parse_lexeme_type(buf);
        int end = buf->cur;

        if(type == LEX_NONE)
        {
            return false;
        }

        Str v = str_sub(str_n$(buf->size, (char *)buf->buffer), beg, end);
        vec_push(&self->str_list, v);

        Lexeme lex = {
            .pos_start = beg,
            .type = type,
            .str = (self->str_list.length - 1),
        };

        vec_push(&self->lexemes, lex);
    }
    return true;
}

#define LEXEME_FOREACH_TABLE_STR(entry, val) [LEX_##entry] = str_const$(#entry),

Str lexeme_table_str[] = {LEXEME_FOREACH(LEXEME_FOREACH_TABLE_STR)};


Str lexeme_to_str(LexemeType type)
{
    return lexeme_table_str[type];
}
