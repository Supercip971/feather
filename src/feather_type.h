#pragma once
#include <stdint.h>

static const char *sys_type[] = {
    "int",
    "string",
    "char",
    "float",
    "address",
    "func",
    "\0",
    "{",
    "[",
    "(",
    ")",
    "]",
    "}",
    "\0"
    "\"",
    "=",
    "*",
    "+",
    "-",
    "/",
    "%",
    ","
    "\0",
    "import",
    "return",
    "if",
    "else",
    "elif",
    "while",
    "for",
};
namespace fsl
{
    struct feather_lexer_item
    {
        int type;
        int subtype = 0;
        unsigned int line = 0;
        unsigned int position;
    };
    struct feather_element_item
    {
        char *name;
        unsigned int line = 0;
        unsigned int position;
    };
    enum feather_type
    {
        FEATHER_P_TYPE = 1,    // int string void ...
        FEATHER_DELIMITOR = 2, // () [] {} ...
        FEATHER_OPERATOR = 3,  // variable name
        FEATHER_TOKEN = 4,     // variable name
        FEATHER_ELEMENT = 5    // return, import, ....
    };
    struct perline_information
    {

        uint64_t current_line;
        uint64_t length;
        uint64_t line_start_context;
        uint64_t litem_length = 0;
        feather_lexer_item *litem[64];
        uint64_t felment_length = 0;
        feather_element_item *felment[32];
    };
} // namespace fsl