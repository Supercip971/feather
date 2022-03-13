#include <stdio.h>
#include <stdlib.h>
#include <feather/filebuf/filebuf.h>
#include <feather/lex/lexeme.h>
#include <feather/utils/log.h>


FileBuf file_read(const char* path)
{
    void* buf;
    FILE* f = fopen(path, "r");

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    buf = malloc(size);

    fread(buf, size, 1, f);
    fclose(f);

    FileBuf fbuf = {};
    fbuf_init(&fbuf, buf, size);
    return fbuf;
}

int main(int argc, char** argv)
{

    if(argc == 1)
    {
        printf("usage: feather [filename] ...\n");
        return 0;
    }

    FileBuf buf = file_read(argv[1]);

    Lexemes lex;

    if(parse_lexemes(&lex, &buf) != true)
    {

        log_error("error while lexing!");
        log_error("at: %i", buf.cur);

        free(buf.buffer);
        return -1;
    }


    for(int i = 0; i < lex.lexemes.length; i++)
    {
        Lexeme cur = lex.lexemes.data[i];
        if(cur.type == LEX_WHITESPACE)
        {
            continue;
        }
        log_info("lex[%i]: %S -> %S", i, lexeme_to_str(cur.type), lex.str_list.data[cur.str]);
    }

    free(buf.buffer);

    return 0;
}
