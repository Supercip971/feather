#pragma once
#include <feather/filebuf/filebuf.h>
#include <feather/utils/str.h>
#include <feather/utils/vec.h>

#define LEXEME_FOREACH(LEXEME)       \
    LEXEME(NONE, "")                 \
    LEXEME(WHITESPACE, "")           \
    LEXEME(COMMENT, "")              \
    LEXEME(NEWLINE, "\n")            \
    LEXEME(IDENT, "")                \
    LEXEME(INTEGER, "")              \
    LEXEME(AUTO, "auto")             \
    LEXEME(BREAK, "break")           \
    LEXEME(CASE, "case")             \
    LEXEME(CHAR, "char")             \
    LEXEME(CONST, "const")           \
    LEXEME(CONTINUE, "continue")     \
    LEXEME(DEFAULT, "default")       \
    LEXEME(DO, "do")                 \
    LEXEME(ELSE, "else")             \
    LEXEME(ENUM, "enum")             \
    LEXEME(EXTERN, "extern")         \
    LEXEME(FLOAT, "float")           \
    LEXEME(FOR, "for")               \
    LEXEME(IF, "if")                 \
    LEXEME(INLINE, "inline")         \
    LEXEME(INT, "int")               \
    LEXEME(LONG, "long")             \
    LEXEME(RETURN, "return")         \
    LEXEME(STATIC, "static")         \
    LEXEME(CLASS, "class")           \
    LEXEME(SWITCH, "switch")         \
    LEXEME(FUNCTION, "function")     \
    LEXEME(UINT, "uint")             \
    LEXEME(VOID, "void")             \
    LEXEME(WHILE, "while")           \
    LEXEME(BOOL, "bool")             \
    LEXEME(LPARENT, "(")             \
    LEXEME(RPARENT, ")")             \
    LEXEME(LBRACKET, "[")            \
    LEXEME(RBRACKET, "]")            \
    LEXEME(LBRACE, "{")              \
    LEXEME(RBRACE, "}")              \
    LEXEME(COMMA, ",")               \
    LEXEME(LCHEVRON_EQ, "<=")        \
    LEXEME(RCHEVRON_EQ, ">=")        \
    LEXEME(ARROW, "->")              \
    LEXEME(LSHIFT_EQ, "<<=")         \
    LEXEME(RSHIFT_EQ, ">>=")         \
    LEXEME(LSHIFT, "<<")             \
    LEXEME(RSHIFT, ">>")             \
    LEXEME(LCHEVRON, "<")            \
    LEXEME(RCHEVRON, ">")            \
    LEXEME(DOUBLE_POUND, "##")       \
    LEXEME(POUND, "#")               \
    LEXEME(PLUSPLUS, "++")           \
    LEXEME(MINUSMINUS, "--")         \
    LEXEME(EQUALEQUAL, "==")         \
    LEXEME(EQUAL, "=")               \
    LEXEME(PLUS_EQ, "+=")  /* += */  \
    LEXEME(MINUS_EQ, "-=") /* -= */  \
    LEXEME(STAR_EQ, "*=")  /* ... */ \
    LEXEME(SLASH_EQ, "/=")           \
    LEXEME(PERCENT_EQ, "%=")         \
    LEXEME(AMPERSAND_EQ, "&=")       \
    LEXEME(BAR_EQ, "|=")             \
    LEXEME(CIRCUMFLEX_EQ, "~=")      \
    LEXEME(STAR, "*")                \
    LEXEME(SLASH, "/")               \
    LEXEME(PERCENT, "%")             \
    LEXEME(AMPERSANDAMPERSAND, "&&") \
    LEXEME(AMPERSAND, "&")           \
    LEXEME(BARBAR, "||")             \
    LEXEME(BAR, "|")                 \
    LEXEME(CIRCUMFLEX, "^")          \
    LEXEME(TILDE, "~")               \
    LEXEME(EXCLAMATIONEQUAL, "!=")   \
    LEXEME(EXCLAMATION, "!")         \
    LEXEME(MINUS, "-")               \
    LEXEME(PLUS, "+")                \
    LEXEME(DOT, ".")                 \
    LEXEME(COLON, ":")               \
    LEXEME(SEMICOLON, ";")           \
    LEXEME(STRING, "")               \
    LEXEME(CHARACTER, "")

#define LEXEME_FOREACH_ENUM(LEXEME, STR) \
    LEX_##LEXEME,

typedef enum
{
    LEXEME_FOREACH(LEXEME_FOREACH_ENUM)
} LexemeType;

typedef struct
{
    int type;
    int str;
    int pos_start;
} Lexeme;

typedef struct
{

    vec_t(Lexeme) lexemes;
    vec_t(Str) str_list;
} Lexemes;

bool parse_lexemes(Lexemes *self, FileBuf *buf);

Str lexeme_to_str(LexemeType type);
